#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>

//
//  Pattern 9 — Iterator
//  Context: The game has several collections
//  (Inventory, EnemyWave, SkillTree) that must
//  be traversed in different ways without exposing
//  their internal storage structure.
//
//  Each collection provides one or more iterators:
//    Inventory    — forward iterator over items
//    EnemyWave    — forward + reverse iterator
//    SkillTree    — depth-first (pre-order) iterator
//
//  IIterator<T>  — iterator interface
//  ICollection<T>— collection interface
//

//  Generic iterator interface
template <typename T>
class IIterator
{
public:
	virtual bool hasNext() const = 0; // true if more elements remain
	virtual T next() = 0;		  // return current and advance
	virtual void reset() = 0;	  // restart from beginning
	virtual ~IIterator() = default;
};

//  Generic collection interface
template <typename T>
class ICollection
{
public:
	virtual std::unique_ptr<IIterator<T>> createIterator() = 0;
	virtual ~ICollection() = default;
};

//  Collection 1: Inventory
//  Simple array-backed bag of items (string name).
//  Provides: ForwardIterator, FilterIterator (by prefix)

class Inventory : public ICollection<std::string>
{
	std::vector<std::string> items_;

public:
	void addItem(const std::string &item) { items_.push_back(item); }
	int size() const { return static_cast<int>(items_.size()); }

	//  Forward iterator
	class ForwardIterator : public IIterator<std::string>
	{
		const std::vector<std::string> &items_;
		int index_ = 0;

	public:
		explicit ForwardIterator(const std::vector<std::string> &items)
		    : items_(items) {}

		bool hasNext() const override { return index_ < static_cast<int>(items_.size()); }
		std::string next() override
		{
			if (!hasNext())
				throw std::out_of_range("Iterator exhausted");
			return items_[index_++];
		}
		void reset() override { index_ = 0; }
	};

	//  Filter iterator — yields only items containing a keyword
	class FilterIterator : public IIterator<std::string>
	{
		const std::vector<std::string> &items_;
		std::string keyword_;
		int index_ = 0;

		void advance() // skip items that don't match
		{
			while (index_ < static_cast<int>(items_.size()) &&
			       items_[index_].find(keyword_) == std::string::npos)
				++index_;
		}

	public:
		FilterIterator(const std::vector<std::string> &items, std::string kw)
		    : items_(items), keyword_(std::move(kw)) { advance(); }

		bool hasNext() const override { return index_ < static_cast<int>(items_.size()); }
		std::string next() override
		{
			if (!hasNext())
				throw std::out_of_range("Iterator exhausted");
			std::string val = items_[index_++];
			advance();
			return val;
		}
		void reset() override
		{
			index_ = 0;
			advance();
		}
	};

	std::unique_ptr<IIterator<std::string>> createIterator() override
	{
		return std::make_unique<ForwardIterator>(items_);
	}

	std::unique_ptr<IIterator<std::string>> createFilterIterator(const std::string &kw)
	{
		return std::make_unique<FilterIterator>(items_, kw);
	}
};

//  Collection 2: EnemyWave
//  Stores enemy structs; supports forward and reverse traversal

struct EnemyInfo
{
	std::string name;
	int hp;
	int damage;
};

class EnemyWave : public ICollection<EnemyInfo>
{
	std::vector<EnemyInfo> enemies_;

public:
	void addEnemy(const EnemyInfo &e) { enemies_.push_back(e); }
	int size() const { return static_cast<int>(enemies_.size()); }

	//  Forward iterator
	class ForwardIterator : public IIterator<EnemyInfo>
	{
		const std::vector<EnemyInfo> &enemies_;
		int index_ = 0;

	public:
		explicit ForwardIterator(const std::vector<EnemyInfo> &e) : enemies_(e) {}

		bool hasNext() const override { return index_ < static_cast<int>(enemies_.size()); }
		EnemyInfo next() override
		{
			if (!hasNext())
				throw std::out_of_range("Iterator exhausted");
			return enemies_[index_++];
		}
		void reset() override { index_ = 0; }
	};

	//  Reverse iterator
	class ReverseIterator : public IIterator<EnemyInfo>
	{
		const std::vector<EnemyInfo> &enemies_;
		int index_;

	public:
		explicit ReverseIterator(const std::vector<EnemyInfo> &e)
		    : enemies_(e), index_(static_cast<int>(e.size()) - 1) {}

		bool hasNext() const override { return index_ >= 0; }
		EnemyInfo next() override
		{
			if (!hasNext())
				throw std::out_of_range("Iterator exhausted");
			return enemies_[index_--];
		}
		void reset() override { index_ = static_cast<int>(enemies_.size()) - 1; }
	};

	std::unique_ptr<IIterator<EnemyInfo>> createIterator() override
	{
		return std::make_unique<ForwardIterator>(enemies_);
	}

	std::unique_ptr<IIterator<EnemyInfo>> createReverseIterator()
	{
		return std::make_unique<ReverseIterator>(enemies_);
	}
};

//  Collection 3: SkillTree
//  N-ary tree of skills. Iterator performs pre-order DFS traversal
//  (root → children left-to-right) without exposing tree internals.

struct SkillNode
{
	std::string skillName;
	int level;
	std::vector<std::shared_ptr<SkillNode>> children;

	SkillNode(std::string n, int lvl) : skillName(std::move(n)), level(lvl) {}

	void addChild(std::shared_ptr<SkillNode> child)
	{
		children.push_back(std::move(child));
	}
};

class SkillTree : public ICollection<SkillNode *>
{
	std::shared_ptr<SkillNode> root_;

public:
	explicit SkillTree(std::shared_ptr<SkillNode> root)
	    : root_(std::move(root)) {}

	//  Pre-order DFS iterator
	class DfsIterator : public IIterator<SkillNode *>
	{
		std::shared_ptr<SkillNode> root_;
		std::vector<SkillNode *> stack_; // explicit DFS stack

		void pushChildren(SkillNode *node)
		{
			// Push in reverse so leftmost child is processed first
			for (int i = static_cast<int>(node->children.size()) - 1; i >= 0; --i)
				stack_.push_back(node->children[i].get());
		}

	public:
		explicit DfsIterator(std::shared_ptr<SkillNode> root) : root_(std::move(root))
		{
			if (root_)
				stack_.push_back(root_.get());
		}

		bool hasNext() const override { return !stack_.empty(); }
		SkillNode *next() override
		{
			if (!hasNext())
				throw std::out_of_range("Iterator exhausted");
			SkillNode *current = stack_.back();
			stack_.pop_back();
			pushChildren(current);
			return current;
		}
		void reset() override
		{
			stack_.clear();
			if (root_)
				stack_.push_back(root_.get());
		}
	};

	std::unique_ptr<IIterator<SkillNode *>> createIterator() override
	{
		return std::make_unique<DfsIterator>(root_);
	}
};
