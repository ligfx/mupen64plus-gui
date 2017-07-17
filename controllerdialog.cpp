#include "controllerdialog.h"
#include "core_interface.h"
#include "plugin.h"
#include "settingclasses.h"
#include "common.h"

#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QTabWidget>
#include <QVBoxLayout>
#include <SDL2/SDL.h>

m64p_handle p1Handle;
m64p_handle p2Handle;
m64p_handle p3Handle;
m64p_handle p4Handle;
QGridLayout* p1Layout;
QGridLayout* p2Layout;
QGridLayout* p3Layout;
QGridLayout* p4Layout;
QStringList joyNames;
int p1Row;
int p2Row;
int p3Row;
int p4Row;
bool p1Auto;
bool p2Auto;
bool p3Auto;
bool p4Auto;

void controllerListCallback(void * context, const char *ParamName, m64p_type ParamType)
{
    if (strcmp(ParamName, "version") == 0 || strcmp(ParamName, "name") == 0)
        return;

    int *myRow = nullptr;
    bool *pAuto = nullptr;
    QGridLayout *my_layout = nullptr;
    m64p_handle current_handle = nullptr;
    if (strcmp((char*)context, "Input-SDL-Control1") == 0) {
        current_handle = p1Handle;
        my_layout = p1Layout;
        myRow = &p1Row;
        pAuto = &p1Auto;
    } else if (strcmp((char*)context, "Input-SDL-Control2") == 0) {
        current_handle = p2Handle;
        my_layout = p2Layout;
        myRow = &p2Row;
        pAuto = &p2Auto;
    } else if (strcmp((char*)context, "Input-SDL-Control3") == 0) {
        current_handle = p3Handle;
        my_layout = p3Layout;
        myRow = &p3Row;
        pAuto = &p3Auto;
    } else if (strcmp((char*)context, "Input-SDL-Control4") == 0) {
        current_handle = p4Handle;
        my_layout = p4Layout;
        myRow = &p4Row;
        pAuto = &p4Auto;
    }

    if (strcmp(ParamName, "mode") == 0) {
        if ((*ConfigGetParamInt)(current_handle, ParamName) == 2) {
            int temp = 1;
            (*ConfigSetParameter)(current_handle, ParamName, ParamType, &temp);
        }
    }

    int l_ParamInt = 0;
    bool l_ParamBool = 0;
    QString l_ParamString;
    QString helper = (*ConfigGetParameterHelp)(current_handle, ParamName);
    QLabel *desc = new QLabel(ParamName);
    if (!helper.isEmpty()) {
        helper.prepend("<span style=\"color:black;\">");
        helper.append("</span>");
        desc->setToolTip(helper);
    }
    desc->setStyleSheet("border: 1px solid; padding: 10px");
    my_layout->addWidget(desc, *myRow, 0);
    switch (ParamType) {
    case M64TYPE_INT:
        l_ParamInt = (*ConfigGetParamInt)(current_handle, ParamName);
        break;
    case M64TYPE_FLOAT:
        break;
    case M64TYPE_BOOL:
        l_ParamBool = (*ConfigGetParamBool)(current_handle, ParamName);
        break;
    case M64TYPE_STRING:
        l_ParamString = (*ConfigGetParamString)(current_handle, ParamName);
        break;
    }
    void *my_Widget;
    if (strcmp(ParamName, "mode") == 0) {
        my_Widget = new CustomComboBox;
        ((CustomComboBox*)my_Widget)->setConfigHandle(current_handle);
        ((CustomComboBox*)my_Widget)->setParamType(ParamType);
        ((CustomComboBox*)my_Widget)->setParamName(ParamName);
        ((CustomComboBox*)my_Widget)->setAuto(pAuto);
        ((CustomComboBox*)my_Widget)->insertItem(0, "Manual");
        ((CustomComboBox*)my_Widget)->insertItem(1, "Automatic");
        ((CustomComboBox*)my_Widget)->setCurrentIndex(l_ParamInt);
        if (l_ParamInt)
            *pAuto = true;
        else
            *pAuto = false;
    }
    else if (strcmp(ParamName, "device") == 0) {
        my_Widget = new CustomComboBox;
        ((CustomComboBox*)my_Widget)->setConfigHandle(current_handle);
        ((CustomComboBox*)my_Widget)->setParamType(ParamType);
        ((CustomComboBox*)my_Widget)->setParamName(ParamName);
        int i;
        for (i = 0; i < joyNames.length(); ++i) {
            ((CustomComboBox*)my_Widget)->insertItem(i, joyNames.at(i) + " (" + QString::number(i) + ")");
        }
        ((CustomComboBox*)my_Widget)->insertItem(i, "Keyboard");
        ((CustomComboBox*)my_Widget)->insertItem(i + 1, "None");
        if (l_ParamInt == -1) {
            if (strcmp((*ConfigGetParamString)(current_handle, "name"), "Keyboard") == 0)
                ((CustomComboBox*)my_Widget)->setCurrentIndex(i);
            else
                ((CustomComboBox*)my_Widget)->setCurrentIndex(i + 1);
        } else
            ((CustomComboBox*)my_Widget)->setCurrentIndex(l_ParamInt);
    }
    else if (strcmp(ParamName, "plugged") == 0) {
        my_Widget = new CustomCheckBox;
        ((CustomCheckBox*)my_Widget)->setConfigHandle(current_handle);
        ((CustomCheckBox*)my_Widget)->setParamType(ParamType);
        ((CustomCheckBox*)my_Widget)->setParamName(ParamName);
        ((CustomCheckBox*)my_Widget)->setCheckState(l_ParamBool ? Qt::Checked : Qt::Unchecked);
        ((CustomComboBox*)my_Widget)->setDisabled(*pAuto);
    }
    else if (strcmp(ParamName, "plugin") == 0) {
        my_Widget = new CustomComboBox;
        ((CustomComboBox*)my_Widget)->setConfigHandle(current_handle);
        ((CustomComboBox*)my_Widget)->setParamType(ParamType);
        ((CustomComboBox*)my_Widget)->setParamName(ParamName);
        ((CustomComboBox*)my_Widget)->insertItem(0, "None");
        ((CustomComboBox*)my_Widget)->insertItem(1, "Mem pak");
        ((CustomComboBox*)my_Widget)->insertItem(2, "Transfer pak");
        ((CustomComboBox*)my_Widget)->insertItem(3, "Rumble pak");
        switch (l_ParamInt) {
        case 1:
            ((CustomComboBox*)my_Widget)->setCurrentIndex(0);
            break;
        case 2:
            ((CustomComboBox*)my_Widget)->setCurrentIndex(1);
            break;
        case 4:
            ((CustomComboBox*)my_Widget)->setCurrentIndex(2);
            break;
        case 5:
            ((CustomComboBox*)my_Widget)->setCurrentIndex(3);
            break;
        default:
            break;
        }
    }
    else if (strcmp(ParamName, "mouse") == 0) {
        my_Widget = new CustomCheckBox;
        ((CustomCheckBox*)my_Widget)->setConfigHandle(current_handle);
        ((CustomCheckBox*)my_Widget)->setParamType(ParamType);
        ((CustomCheckBox*)my_Widget)->setParamName(ParamName);
        ((CustomCheckBox*)my_Widget)->setCheckState(l_ParamBool ? Qt::Checked : Qt::Unchecked);
        ((CustomCheckBox*)my_Widget)->setDisabled(*pAuto);
    }
    else if (strcmp(ParamName, "MouseSensitivity") == 0) {
        my_Widget = new CustomLineEdit;
        ((CustomLineEdit*)my_Widget)->setConfigHandle(current_handle);
        ((CustomLineEdit*)my_Widget)->setParamType(ParamType);
        ((CustomLineEdit*)my_Widget)->setParamName(ParamName);
        ((CustomLineEdit*)my_Widget)->setStyleSheet("border: 1px solid; padding: 10px");
        ((CustomLineEdit*)my_Widget)->setText(l_ParamString);
        ((CustomLineEdit*)my_Widget)->setDisabled(*pAuto);
    }
    else if (strcmp(ParamName, "AnalogDeadzone") == 0) {
        my_Widget = new CustomLineEdit;
        ((CustomLineEdit*)my_Widget)->setConfigHandle(current_handle);
        ((CustomLineEdit*)my_Widget)->setParamType(ParamType);
        ((CustomLineEdit*)my_Widget)->setParamName(ParamName);
        ((CustomLineEdit*)my_Widget)->setStyleSheet("border: 1px solid; padding: 10px");
        ((CustomLineEdit*)my_Widget)->setText(l_ParamString);
        ((CustomLineEdit*)my_Widget)->setDisabled(*pAuto);
    }
    else if (strcmp(ParamName, "AnalogPeak") == 0) {
        my_Widget = new CustomLineEdit;
        ((CustomLineEdit*)my_Widget)->setConfigHandle(current_handle);
        ((CustomLineEdit*)my_Widget)->setParamType(ParamType);
        ((CustomLineEdit*)my_Widget)->setParamName(ParamName);
        ((CustomLineEdit*)my_Widget)->setStyleSheet("border: 1px solid; padding: 10px");
        ((CustomLineEdit*)my_Widget)->setText(l_ParamString);
        ((CustomLineEdit*)my_Widget)->setDisabled(*pAuto);
    }
    else if (strcmp(ParamName, "GBRomFile") == 0 || strcmp(ParamName, "GBSaveFile") == 0) {
        my_Widget = new CustomPushButton2;
        ((CustomPushButton2*)my_Widget)->setConfigHandle(current_handle);
        ((CustomPushButton2*)my_Widget)->setParamType(ParamType);
        ((CustomPushButton2*)my_Widget)->setParamName(ParamName);
        ((CustomPushButton2*)my_Widget)->setText(l_ParamString);
    }
    else {
        my_Widget = new CustomPushButton;
        ((CustomPushButton*)my_Widget)->setConfigHandle(current_handle);
        ((CustomPushButton*)my_Widget)->setParamType(ParamType);
        ((CustomPushButton*)my_Widget)->setParamName(ParamName);
        ((CustomPushButton*)my_Widget)->setText(l_ParamString);
        ((CustomPushButton*)my_Widget)->setDisabled(*pAuto);
        ((CustomPushButton*)my_Widget)->setJoystick((*ConfigGetParamInt)(current_handle, "device"));
    }
    my_layout->addWidget((QWidget*)my_Widget, *myRow, 1);
    ++*myRow;
}

void ControllerDialog::handleResetButton()
{
    int value;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &value);
    if (value == M64EMU_STOPPED) {
        (*ConfigDeleteSection)("Input-SDL-Control1");
        (*ConfigDeleteSection)("Input-SDL-Control2");
        (*ConfigDeleteSection)("Input-SDL-Control3");
        (*ConfigDeleteSection)("Input-SDL-Control4");
        (*ConfigSaveFile)();
        (*CoreShutdown)();
        (*DetachCoreLib)();
        this->close();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Emulator must be stopped.");
        msgBox.exec();
    }
}

ControllerDialog::ControllerDialog()
{
    int value;
    (*CoreDoCommand)(M64CMD_CORE_STATE_QUERY, M64CORE_EMU_STATE, &value);
    if (value == M64EMU_STOPPED) {
        PluginSearchLoad();
        PluginUnload();
    }
    if (!SDL_WasInit(SDL_INIT_JOYSTICK)) {
        if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) == -1)
        {
            DebugMessage(M64MSG_ERROR, "Couldn't init SDL joystick subsystem: %s", SDL_GetError() );
        }
    }
    int numJoysticks = SDL_NumJoysticks();
    joyNames.clear();
    int i;
    for (i = 0; i < numJoysticks; ++i)
        joyNames.append(SDL_JoystickName(SDL_JoystickOpen(i)));
    p1Row = 0;
    p2Row = 0;
    p3Row = 0;
    p4Row = 0;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->setUsesScrollButtons(false);

    QWidget *p1Settings = new QWidget;
    p1Layout = new QGridLayout;
    p1Settings->setLayout(p1Layout);
    (*ConfigOpenSection)("Input-SDL-Control1", &p1Handle);
    (*ConfigListParameters)(p1Handle, (char*)"Input-SDL-Control1", controllerListCallback);
    QScrollArea *p1Scroll = new QScrollArea;
    p1Scroll->setWidget(p1Settings);
    p1Scroll->setMinimumWidth(p1Settings->sizeHint().width() + 20);
    p1Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabWidget->addTab(p1Scroll, tr("Player 1"));

    QWidget *p2Settings = new QWidget;
    p2Layout = new QGridLayout;
    p2Settings->setLayout(p2Layout);
    (*ConfigOpenSection)("Input-SDL-Control2", &p2Handle);
    (*ConfigListParameters)(p2Handle, (char*)"Input-SDL-Control2", controllerListCallback);
    QScrollArea *p2Scroll = new QScrollArea;
    p2Scroll->setWidget(p2Settings);
    p2Scroll->setMinimumWidth(p2Settings->sizeHint().width() + 20);
    p2Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabWidget->addTab(p2Scroll, tr("Player 2"));

    QWidget *p3Settings = new QWidget;
    p3Layout = new QGridLayout;
    p3Settings->setLayout(p3Layout);
    (*ConfigOpenSection)("Input-SDL-Control3", &p3Handle);
    (*ConfigListParameters)(p3Handle, (char*)"Input-SDL-Control3", controllerListCallback);
    QScrollArea *p3Scroll = new QScrollArea;
    p3Scroll->setWidget(p3Settings);
    p3Scroll->setMinimumWidth(p3Settings->sizeHint().width() + 20);
    p3Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabWidget->addTab(p3Scroll, tr("Player 3"));

    QWidget *p4Settings = new QWidget;
    p4Layout = new QGridLayout;
    p4Settings->setLayout(p4Layout);
    (*ConfigOpenSection)("Input-SDL-Control4", &p4Handle);
    (*ConfigListParameters)(p4Handle, (char*)"Input-SDL-Control4", controllerListCallback);
    QScrollArea *p4Scroll = new QScrollArea;
    p4Scroll->setWidget(p4Settings);
    p4Scroll->setMinimumWidth(p4Settings->sizeHint().width() + 20);
    p4Scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabWidget->addTab(p4Scroll, tr("Player 4"));

    mainLayout->addWidget(tabWidget);
    QPushButton *resetButton = new QPushButton("Reset All Settings");
    connect(resetButton, SIGNAL (released()),this, SLOT (handleResetButton()));
    mainLayout->addWidget(resetButton);
    setLayout(mainLayout);
}
