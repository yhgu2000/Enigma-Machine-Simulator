#include "EnigmaMachine.hpp"
#include "RotorSeparator.hpp"
#include "RotorSlot.hpp"
#include "ui_EnigmaMachine.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QToolTip>
#include <QtMultimedia/QSound>
#include <iostream>

EnigmaMachine::EnigmaMachine(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::EnigmaMachine)
{
  ui->setupUi(this);
}

EnigmaMachine::~EnigmaMachine()
{
  delete ui;
}

void
EnigmaMachine::step()
{
  for (auto i : _rotorSlots) {
    if (!i->step())
      break;
  }
}

void
EnigmaMachine::step_back()
{
  for (auto i : _rotorSlots) {
    if (!i->step_back())
      break;
  }
}

void
EnigmaMachine::set_focus()
{
  ui->machineIn->setFocus();
}

void
EnigmaMachine::retranslate_ui()
{
  ui->retranslateUi(this);
  for (auto i : _rotorSlots)
    i->retranslate_ui();
}

void
EnigmaMachine::on_machineIn_textEdited(const QString& arg1)
{
  static QSound sKeySound(":/key.wav");
  // 从用户按键到 textEdited 经过太多环节，最好移到组件的 keyPressedEvent 里面去
  sKeySound.play();

  if (arg1.size() == 0) {
    ui->machineIn->setText(" ");

    // 后退一步
    step_back();

    emit backspace();
    return;
  }

  QChar plain = arg1.at(arg1.size() - 1);
  if (!plain.isLetter()) {
    ui->machineIn->clear();
    QToolTip::showText(
      ui->machineIn->mapToGlobal(QPoint(0, ui->machineIn->height() >> 1)),
      tr("⚠ enigma only accepts letters"));
    return;
  }
  ui->machineIn->setText(plain);

  // 推进一步
  step();

  bool isLower = plain.isLower();
  uint8_t x;
  if (isLower)
    x = uint8_t(plain.toLatin1() - 'a');
  else
    x = uint8_t(plain.toLatin1() - 'A');

  // 正射
  for (int i = 0; i < _rotorSlots.size(); ++i) {
    auto& rsl = *(_rotorSlots[i]);
    auto& rse = *(_rotorSeps[i]);

    rse.set_wireL2R(x);
    x = rsl.forward_map(x);
  }

  // 反射器
  ui->reflectorIn->setText(QChar(x + 'A'));
  x = _reflector[x];
  ui->reflectorOut->setText(QChar(x + 'A'));

  // 反射
  for (auto i = _rotorSlots.size(); --i != -1;) {
    auto& rsl = *(_rotorSlots[i]);
    auto& rse = *(_rotorSeps[i]);

    x = rsl.reverse_map(x);
    rse.set_wireR2L(x);
  }

  // 输出
  QChar cipher;
  if (isLower)
    cipher = QChar(x + 'a');
  else
    cipher = QChar(x + 'A');

  ui->machineOut->setText(plain);

  emit charEncoded(plain, cipher);
}

void
EnigmaMachine::on_newRotorButton_clicked()
{
  auto rsl = new RotorSlot();
  auto rse = new RotorSeparator();

  _rotorSlots.push_front(rsl);
  _rotorSeps.push_front(rse);

  ui->rotorSetLayout->insertWidget(0, rsl);
  ui->rotorSetLayout->insertWidget(0, rse);
}

void
EnigmaMachine::on_editReflectorButton_clicked()
{
  EditRotorDialog dlg;
  dlg.setWindowTitle(tr("REFLECTOR"));
  dlg.set_rotor(_reflector);
  if (!dlg.exec())
    return;
  _reflector = dlg.get_rotor();
}

void
EnigmaMachine::on_trashButton_clicked()
{
  // TODO 拖动删除
  if (!_rotorSlots.isEmpty()) {
    delete _rotorSeps.front();
    delete _rotorSlots.front();
    _rotorSlots.pop_front();
    _rotorSeps.pop_front();
  }
}

void
EnigmaMachine::on_loadButton_clicked()
{
  auto s = QFileDialog::getOpenFileName(nullptr,
                                        tr("Import from File ..."),
                                        "",
                                        tr("Enigma Machine Status (*.ems)"));
  if (s.isEmpty())
    return;

  QFile f(s);
  if (!f.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::critical(nullptr, tr("Load Failed"), tr("cannot open file"));
    return;
  }

  QTextStream fin(&f);
  int rotorNumber = 0;
  fin >> rotorNumber;

  Rotor reflector;
  QVector<Rotor> rotors(rotorNumber);
  QVector<int> offsets(rotorNumber);

  QString tmp;
  fin >> tmp;
  if (!reflector.decode(tmp))
    goto DECODING_FAILED;

  for (int i = 0; i < rotorNumber; ++i) {
    fin >> offsets[i];
    fin >> tmp;
    if (!rotors[i].decode(tmp))
      goto DECODING_FAILED;
  }
  if (fin.status() != fin.Ok)
    goto DECODING_FAILED;

  // 调整窗口里的转子槽数
  for (int i = _rotorSlots.size(); i < rotorNumber; ++i) {
    _rotorSeps.push_front(new RotorSeparator());
    _rotorSlots.push_front(new RotorSlot());
    ui->rotorSetLayout->insertWidget(0, _rotorSlots.front());
    ui->rotorSetLayout->insertWidget(0, _rotorSeps.front());
  }
  for (int i = rotorNumber; i < _rotorSlots.size(); ++i) {
    delete _rotorSeps.front();
    delete _rotorSlots.front();
    _rotorSeps.pop_front();
    _rotorSlots.pop_front();
  }

  // 设置机器状态
  for (int i = 0; i < rotorNumber; ++i) {
    _rotorSlots[i]->set_rotor(rotors[i]);
    _rotorSlots[i]->set_offset(uint8_t(offsets[i]));
  }
  QMessageBox::information(nullptr, tr("Success"), tr("macine status loaded"));
  return;

DECODING_FAILED:
  QMessageBox::critical(nullptr, tr("Load Failed"), tr("fail to dncode file"));
}

void
EnigmaMachine::on_saveButton_clicked()
{
  auto s = QFileDialog::getSaveFileName(
    nullptr, tr("Export to File ..."), "", tr("Enigma Machine Status (*.ems)"));
  if (s.isEmpty())
    return;

  QFile f(s);
  if (!f.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::critical(nullptr, tr("Save Failed"), tr("cannot open file"));
    return;
  }

  QTextStream fout(&f);

  QString tmp;
  if (!_reflector.encode(tmp))
    goto ENCODING_FAILED;
  fout << _rotorSlots.size() << '\t' << tmp << Qt::endl;

  for (auto i : _rotorSlots) {
    if (!i->get_rotor().encode(tmp))
      goto ENCODING_FAILED;
    fout << i->get_offset() << '\t' << tmp << Qt::endl;
  }

  if (fout.status() != QTextStream::Ok)
    goto ENCODING_FAILED;

  f.close();
  QMessageBox::information(this, tr("Success"), tr("File Saved"));
  return;

ENCODING_FAILED:
  QMessageBox::critical(
    nullptr, tr("Save Failed"), tr("fail to encode machine"));
}
