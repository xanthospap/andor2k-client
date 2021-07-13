#include "mainoptionwindow.h"
#include <QApplication>
#include <QPushButton>

MainOptionWindow::MainOptionWindow(QWidget *parent) : QWidget(parent) {
  // set size of window
  setFixedSize(300, 200);

  createGui();
  setLayout(layout);
  setWindowTitle("Andor2k Client");
}

void MainOptionWindow::createGui() {
  m_filename_ledit = new QLineEdit;
  m_exposure_ledit = new QLineEdit;
  m_nimages_ledit = new QLineEdit;
  m_binning_ledit = new QLineEdit;

  label = new QLabel;
  label->setFrameStyle(QFrame::Box | QFrame::Plain);

  m_exit_button = new QPushButton("Exit");

  m_start_button = new QPushButton("Start", this);
  m_start_button->setEnabled(true);

  layout = new QGridLayout;
  layout->addWidget(new QLabel(tr("Filename:")), 0, 0);
  layout->addWidget(m_filename_ledit, 0, 1);
  layout->addWidget(new QLabel(tr("Exposure:")), 1, 0);
  layout->addWidget(m_exposure_ledit, 1, 1);
  layout->addWidget(new QLabel(tr("Num Images:")), 2, 0);
  layout->addWidget(m_nimages_ledit, 2, 1);
  layout->addWidget(new QLabel(tr("Binning")), 3, 0);
  layout->addWidget(m_binning_ledit, 3, 1);
  layout->addWidget(m_start_button, 4, 0);
  layout->addWidget(m_exit_button, 4, 1);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  // exit signals, closes app
  connect(m_exit_button, SIGNAL(clicked()), QApplication::instance(),
          SLOT(quit()));
}
