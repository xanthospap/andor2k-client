#ifndef ADVANCEDSETTINGS_H
#define ADVANCEDSETTINGS_H

#include <QWidget>

class AdvancedSettings : public QWidget {
  Q_OBJECT
public:
  explicit AdvancedSettings(QWidget *parent = nullptr);

private:
  void createGui();
signals:
};

#endif // ADVANCEDSETTINGS_H
