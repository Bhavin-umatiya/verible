// Copyright 2017-2020 The Verible Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef VERIBLE_COMMON_UTIL_LOGGING_H_
#define VERIBLE_COMMON_UTIL_LOGGING_H_

#ifdef __GNUC__
// b/246413374
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#include "absl/log/check.h"
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "absl/log/die_if_null.h"
#include "absl/log/log.h"

// The VLOG level is set from the VERILOG_VLOG_DETAIL environment variable in
// InitCommandLine(), which calls the VERIBLE_INTERNAL_SET_VLOGLEVEL() macro.
#if defined(VLOG) && defined(ABSL_VLOG_IS_ON)
// If there is an ABSL implementation of VLOG, use that.
#define VERIBLE_INTERNAL_SET_VLOGLEVEL(level) absl::SetGlobalVLogLevel(level)
#else
// ... otherwise provide a minimal local implementation.
namespace verible {
// Used in the VLOG macro to check logging condition.
extern int global_vlog_level_;
}  // namespace verible

#define VERIBLE_INTERNAL_SET_VLOGLEVEL(level) \
  verible::global_vlog_level_ = level

#define VLOG_IS_ON(x) (::verible::global_vlog_level_ >= (x))
#define VLOG(x) LOG_IF(INFO, VLOG_IS_ON(x))

#ifdef NDEBUG
#define DVLOG(x) LOG_IF(INFO, false)
#else
#define DVLOG(x) VLOG(x)
#endif  // NDEBUG
#endif  // VLOG

#define CHECK_NOTNULL(p) (void)ABSL_DIE_IF_NULL(p)

#endif  // VERIBLE_COMMON_UTIL_LOGGING_H_
