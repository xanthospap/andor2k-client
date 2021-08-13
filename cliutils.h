#ifndef CLIUTILS_H
#define CLIUTILS_H

#include <QStringList>
#include <unistd.h>
#include <cstring>

inline auto split_command(const QString &response) noexcept {
  return response.split(';');
}

inline auto get_val(const char *key, const QStringList &list) {
  auto key_sz = std::strlen(key);
  for (const auto &s : list) {
    if (!std::strncmp(key, s.toStdString().c_str(), key_sz)) {
      return QString(s.toStdString().c_str() + key_sz + 1);
      // printf("\tget_val called; returning %s\n", s.toStdString().c_str());
    }
  }
  return QString("xxxx");
}

inline auto get_username_or(const char *uname) noexcept {
  char usern[32];
  QString user;

  return getlogin_r(usern, 32) ? QString(uname) : QString(usern);

  /*
  if (getlogin_r(usern, 32)) {
    user = "andor2k";
  } else {
    user = QString(usern);
  }
  */
}

#endif
