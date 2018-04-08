#pragma once
#include <QObject>

// this is for QVariant conversion mainly
namespace sc3ntist {
Q_NAMESPACE
enum class VariableRefType { GlobalVar, Flag };
Q_ENUM_NS(VariableRefType)
}  // namespace sc3ntist

using namespace sc3ntist;