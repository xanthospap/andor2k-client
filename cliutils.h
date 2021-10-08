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
#ifdef DEBUG
  printf(">> trying to convert to int the string: \"%s\" (%s)\n", get_val("error", list).toStdString().c_str(), __func__);
#endif
  if (converted && error) return error;
  return 0;
}

inline bool list_signals_done(const QStringList &list, int *error=nullptr) {
#ifdef DEBUG
  printf(">> searching response for done/error (%s)\n", __func__);
#endif
  if (error) *error = 0;
  for (const auto &s : list) {
    if (!std::strncmp("done", s.toStdString().c_str(), 4)) {
#ifdef DEBUG
      printf(">> done found! daemon signal end of process (%s)\n", __func__);
#endif
      if (error) {
#ifdef DEBUG
        printf(">> searching for error value, which is: %d (%s)\n", list_has_error(list), __func__);
#endif
        *error = list_has_error(list);
      }
      return true;
    }
  }
  return false;
}

inline auto get_username_or(const char *uname) noexcept {
  char usern[32];
  QString user;

  return getlogin_r(usern, 32) ? QString(uname) : QString(usern);
}

#endif
