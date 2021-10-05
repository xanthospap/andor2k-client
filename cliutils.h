#ifndef CLIUTILS_H
#define CLIUTILS_H

#include <QStringList>
#include <cstring>
#include <unistd.h>

// split a string into (string) parts, using ';' as delimeter
inline auto split_command(const QString &response) noexcept {
  return response.split(';');
}

inline auto get_val(const char *key, const QStringList &list) {
  auto key_sz = std::strlen(key);
  for (const auto &s : list) {
    if (!std::strncmp(key, s.toStdString().c_str(), key_sz)) {
      return QString(s.toStdString().c_str() + key_sz + 1);
    }
  }
  return QString(" ");
}

inline auto list_has_error(const QStringList &list) {
  bool converted;
  int error = get_val("error", list).toInt(&converted);
  if (converted && error) return error;
  return 0;
}

inline auto get_username_or(const char *uname) noexcept {
  char usern[32];
  QString user;

  return getlogin_r(usern, 32) ? QString(uname) : QString(usern);
}

#endif
