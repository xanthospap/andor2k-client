#ifndef EXPOSURETAB_H
#define EXPOSURETAB_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include <QWidget>

class ExposureTab : public QWidget {
  Q_OBJECT
public:
  explicit ExposureTab(QWidget *parent = nullptr);

private:
  void createGui();

  QPushButton *m_exit_button;
  QPushButton *m_start_button;
  QLineEdit *m_filename_ledit;
  QLineEdit *m_exposure_ledit;
  QLineEdit *m_nimages_ledit;
  QLineEdit *m_binning_ledit;
  QLabel *m_label;
  QGridLayout *m_layout;

signals:
};

#endif // EXPOSURETAB_H
