#include "temperaturetab.h"
#include <QApplication>

TemperatureTab::TemperatureTab(QWidget *parent) : QWidget(parent) {
  createGui();
  setLayout(m_layout);
}

void TemperatureTab::createGui() {
  m_set_temp_ledit = new QLineEdit;
  m_show_temp_ledit = new QLineEdit;

  m_label = new QLabel;
  m_label->setFrameStyle(QFrame::Box | QFrame::Plain);

  m_set_button = new QPushButton("Set");

  m_layout = new QGridLayout;
  m_layout->addWidget(new QLabel(tr("Current Temperature (C):")), 0, 0);
  m_layout->addWidget(m_show_temp_ledit, 0, 1);
  m_layout->addWidget(new QLabel(tr("Set Temperature to (C):")), 1, 0);
  m_layout->addWidget(m_set_temp_ledit, 1, 1);
  m_layout->addWidget(m_set_button, 2, 0);
  m_layout->setSizeConstraint(QLayout::SetFixedSize);
}
