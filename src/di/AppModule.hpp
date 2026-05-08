#pragma once

#include "format/IReportFormatter.hpp"
#include "format/JsonReportFormatter.hpp"
#include "repository/IUserRepository.hpp"
#include "repository/JsonUserRepository.hpp"

#include <boost/di.hpp>

namespace di = boost::di;

inline auto createAppInjector() {
    return di::make_injector(
        di::bind<IUserRepository>.to<JsonUserRepository>().in(di::singleton),
        di::bind<IReportFormatter>.to<JsonReportFormatter>().in(di::singleton)
    );
}
