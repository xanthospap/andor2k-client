#ifndef TEMPERATURETAB_H
#define TEMPERATURETAB_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class TemperatureTab : public QWidget {
  Q_OBJECT
public:
  explicit TemperatureTab(QWidget *parent = nullptr);

  void createGui();

  QPushButton *m_set_button;
  QLineEdit *m_set_temp_ledit;
  QLineEdit *m_show_temp_ledit;
  QLabel *m_label;
  QGridLayout *m_layout;

signals:
};

#endif // TEMPERATURETAB_H
