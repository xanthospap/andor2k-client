#include "exposuretab.h"
#include <QApplication>
#include <QStringList>

ExposureTab::ExposureTab(QWidget *parent) : QWidget(parent) {
  createGui();
  setLayout(m_layout);
}

void ExposureTab::createGui() {
  m_filename_ledit = new QLineEdit;
  m_exposure_ledit = new QLineEdit;
  m_nimages_ledit = new QLineEdit;
  m_binning_ledit = new QLineEdit;

  m_label = new QLabel;
  m_label->setFrameStyle(QFrame::Box | QFrame::Plain);

  m_exit_button = new QPushButton("Exit", this);

  m_start_button = new QPushButton("Start", this);
  m_start_button->setEnabled(true);

  QStringList types = {"flat", "object", "bias"};
  m_type_cbox = new QComboBox(this);
  m_type_cbox->addItems(types);

  m_layout = new QGridLayout;
  m_layout->addWidget(new QLabel(tr("Filename:")), 0, 0);
  m_layout->addWidget(m_filename_ledit, 0, 1);
  m_layout->addWidget(new QLabel(tr("Type:")), 1, 0);
  m_layout->addWidget(m_type_cbox, 1, 1);
  m_layout->addWidget(new QLabel(tr("Exposure:")), 2, 0);
  m_layout->addWidget(m_exposure_ledit, 2, 1);
  m_layout->addWidget(new QLabel(tr("Num Images:")), 3, 0);
  m_layout->addWidget(m_nimages_ledit, 3, 1);
  m_layout->addWidget(new QLabel(tr("Binning")), 4, 0);
  m_layout->addWidget(m_binning_ledit, 4, 1);
  m_layout->addWidget(m_start_button, 5, 0);
  m_layout->addWidget(m_exit_button, 5, 1);
  m_layout->setSizeConstraint(QLayout::SetFixedSize);

  // exit signals, closes app
  connect(m_exit_button, SIGNAL(clicked()), QApplication::instance(),
          SLOT(quit()));
}
