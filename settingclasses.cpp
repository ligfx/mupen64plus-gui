#include "settingclasses.h"
#include "core_interface.h"
#include "controllerdialog.h"
#include "keyselect.h"
#include "cheat.h"
#include "common.h"
#include <SDL2/SDL.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>

CustomLineEdit::CustomLineEdit()
{
    connect(this, &QLineEdit::editingFinished, [=](){
        int i_value = this->text().toInt();
        float f_value = this->text().toFloat();
        switch (m_ParamType) {
        case M64TYPE_INT:
            (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, &i_value);
            break;
        case M64TYPE_FLOAT:
            (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, &f_value);
            break;
        case M64TYPE_STRING:
            (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, this->text().toLatin1().data());
            break;
        default:
            break;
        }
        (*ConfigSaveFile)();
    });
}

CustomCheckBox::CustomCheckBox()
{
    connect(this, &QCheckBox::stateChanged, [=](int state){
        int value = state == Qt::Checked ? 1 : 0;
        (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, &value);
        (*ConfigSaveFile)();
    });
}

CheatCheckBox::CheatCheckBox()
{
    m_Checked = false;
    connect(this, &QAbstractButton::clicked, [=](bool checked){
        sCheatInfo *pCheat;
        pCheat = CheatFindCode(m_Number);
        if (checked && !m_Checked) {
            if (pCheat == NULL)
               DebugMessage(M64MSG_WARNING, "invalid cheat code number %i", m_Number);
            else
            {
                if (pCheat->VariableLine != -1 && pCheat->Count > pCheat->VariableLine && m_Option < pCheat->Codes[pCheat->VariableLine].var_count)
                    pCheat->Codes[pCheat->VariableLine].var_to_use = m_Option;
                CheatActivate(pCheat);
                pCheat->active = true;
            }
            m_Checked = true;
        }
        else {
            (*CoreCheatEnabled)(pCheat->Name, 0);
            pCheat->active = false;
            if (m_ButtonGroup != nullptr)
                m_ButtonGroup->setChecked(true);
        }
    });
    connect(this, &QCheckBox::stateChanged, [=](int state){
        int value = state == Qt::Checked ? 1 : 0;
        if (!value)
            m_Checked = false;
    });
}

CustomComboBox::CustomComboBox()
{
    m_Auto = nullptr;
    connect(this, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), [=](int index) {
        int temp;
        if (m_ParamName == "device") {
            if (index == this->count() - 2) {
                temp = -1;
                (*ConfigSetParameter)(m_CurrentHandle, "name", M64TYPE_STRING, "Keyboard");
            } else if (index == this->count() - 1) {
                temp = -1;
                (*ConfigSetParameter)(m_CurrentHandle, "name", M64TYPE_STRING, "");
            } else {
                temp = index;
                (*ConfigSetParameter)(m_CurrentHandle, "name", M64TYPE_STRING, SDL_JoystickName(SDL_JoystickOpen(index)));
            }
        } else if (m_ParamName == "mode") {
            if (index)
                *m_Auto = true;
            else
                *m_Auto = false;
            temp = index;
        } else if (m_ParamName == "plugin") {
            switch (index) {
            case 0:
                temp = 1;
                break;
            case 1:
                temp = 2;
                break;
            case 2:
                temp = 4;
                break;
            case 3:
                temp = 5;
                break;
            default:
                break;
            }
        }
        (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, &temp);
        if (m_ParamName == "mode" || m_ParamName == "device") {
            p1Row = 0;
            p2Row = 0;
            p3Row = 0;
            p4Row = 0;
            (*ConfigListParameters)(p1Handle, (char*)"Input-SDL-Control1", controllerListCallback);
            (*ConfigListParameters)(p2Handle, (char*)"Input-SDL-Control2", controllerListCallback);
            (*ConfigListParameters)(p3Handle, (char*)"Input-SDL-Control3", controllerListCallback);
            (*ConfigListParameters)(p4Handle, (char*)"Input-SDL-Control4", controllerListCallback);
        }
        (*ConfigSaveFile)();
    });
}

CustomPushButton::CustomPushButton()
{
    connect(this, &QAbstractButton::clicked, [=](){
        KeySelect* keyselect = new KeySelect;
        keyselect->setJoystick(m_Joystick);
        keyselect->setParamName(m_ParamName.c_str());
        keyselect->setConfigHandle(m_CurrentHandle);
        keyselect->setButton(this);
        QHBoxLayout* layout = new QHBoxLayout;
        QString myString;
        if (m_ParamName == "X Axis") {
            keyselect->setAxis(true);
            myString.append("Press the key/button you would like to bind to <b>");
            myString.append(QString::fromStdString(m_ParamName));
            myString.append(" LEFT</b> followed by <b>");
            myString.append(QString::fromStdString(m_ParamName));
            myString.append(" RIGHT");
        } else if (m_ParamName == "Y Axis") {
            keyselect->setAxis(true);
            myString.append("Press the key/button you would like to bind to <b>");
            myString.append(QString::fromStdString(m_ParamName));
            myString.append(" UP</b> followed by <b>");
            myString.append(QString::fromStdString(m_ParamName));
            myString.append(" DOWN");
        } else {
            keyselect->setAxis(false);
            myString.append("Press the key/button you would like to bind to <b>");
            myString.append(QString::fromStdString(m_ParamName));
        }
        myString.append("</b>");
        QLabel* label = new QLabel(myString);
        layout->addWidget(label);
        keyselect->setLayout(layout);
        keyselect->show();
    });
}

CustomPushButton2::CustomPushButton2()
{
    connect(this, &QAbstractButton::clicked, [=](){
        QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Select File"), NULL, NULL);
        if (!filename.isNull()) {
            (*ConfigSetParameter)(m_CurrentHandle, m_ParamName.c_str(), m_ParamType, filename.toLatin1().data());
            (*ConfigSaveFile)();
            this->setText(filename);
        }
    });

}
