#ifndef MAINOPTIONWINDOW_H
#define MAINOPTIONWINDOW_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

class QPushButton;
class MainOptionWindow : public QWidget {
  Q_OBJECT
public:
  explicit MainOptionWindow(QWidget *parent = nullptr);

  // signals:
private:
  void createGui();

  QPushButton *m_exit_button;
  QPushButton *m_start_button;
  QLineEdit *m_filename_ledit;
  QLineEdit *m_exposure_ledit;
  QLineEdit *m_nimages_ledit;
  QLineEdit *m_binning_ledit;
  QLabel *label;
  QGridLayout *layout;
};

#endif // MAINOPTIONWINDOW_H
