// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/base/validation.h"

#include <atomic>

#include "flutter/fml/logging.h"

namespace impeller {

static std::atomic_int32_t sValidationLogsDisabledCount = 0;
static std::atomic_int32_t sValidationLogsAreFatal = 0;

void ImpellerValidationErrorsSetFatal(bool fatal) {
  sValidationLogsAreFatal = fatal;
}

ScopedValidationDisable::ScopedValidationDisable() {
  sValidationLogsDisabledCount++;
}

ScopedValidationDisable::~ScopedValidationDisable() {
  sValidationLogsDisabledCount--;
}

ScopedValidationFatal::ScopedValidationFatal() {
  sValidationLogsAreFatal++;
}

ScopedValidationFatal::~ScopedValidationFatal() {
  sValidationLogsAreFatal--;
}

ValidationLog::ValidationLog() = default;

ValidationLog::~ValidationLog() {
  if (sValidationLogsDisabledCount <= 0) {
    ImpellerValidationBreak(stream_.str().c_str());
  }
}

std::ostream& ValidationLog::GetStream() {
  return stream_;
}

void ImpellerValidationBreak(const char* message) {
// Nothing to do. Exists for the debugger.
#ifdef IMPELLER_ENABLE_VALIDATION
  std::stringstream stream;
  stream << "Break on '" << __FUNCTION__
         << "' to inspect point of failure: " << message;
  if (sValidationLogsAreFatal > 0) {
    FML_LOG(FATAL) << stream.str();
  } else {
    FML_LOG(ERROR) << stream.str();
  }
#endif  // IMPELLER_ENABLE_VALIDATION
}

}  // namespace impeller
