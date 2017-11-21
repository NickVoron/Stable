#pragma once

#define COMPONENT_MODEL_KERNEL_DEBUG
#define COMPONENT_MODEL_REPORT_STATS

#ifdef COMPONENT_MODEL_KERNEL_DEBUG
#include "stuff/library.include.h"
#define CM_KERNEL_ENFORCE(exp) ENFORCE(exp)
#define CM_KERNEL_ENFORCE_MSG(exp, msg) ENFORCE_MSG(exp, msg)
#else
#define CM_KERNEL_ENFORCE(exp) (void(0))
#define CM_KERNEL_ENFORCE_MSG(exp, msg) (void(0))
#endif

#define CM_KERNEL_THROW(exp) do { throw Base::Errors::Simple(str::stringize(exp).str()); } while (0);



