#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "m64p_types.h"

class CustomLineEdit : public QLineEdit
{
public:
    CustomLineEdit();
    void setParamName(const char* ParamName) {
        m_ParamName = ParamName;
    }
    void setParamType(m64p_type ParamType) {
        m_ParamType = ParamType;
    }
    void setConfigHandle(m64p_handle CurrentHandle) {
        m_CurrentHandle = CurrentHandle;
    }

private:
    m64p_type m_ParamType;
    std::string m_ParamName;
    m64p_handle m_CurrentHandle;
};

class CustomCheckBox : public QCheckBox
{
public:
    CustomCheckBox();
    void setParamName(const char* ParamName) {
        m_ParamName = ParamName;
    }
    void setParamType(m64p_type ParamType) {
        m_ParamType = ParamType;
    }
    void setConfigHandle(m64p_handle CurrentHandle) {
        m_CurrentHandle = CurrentHandle;
    }

private:
    m64p_type m_ParamType;
    std::string m_ParamName;
    m64p_handle m_CurrentHandle;
};

class CheatCheckBox : public QCheckBox
{
public:
    CheatCheckBox();
    void setNumber(int num) {
        m_Number = num;
    }
    void setOption(int opt) {
        m_Option = opt;
    }
    void setButtonGroup (QCheckBox* bgroup) {
        m_ButtonGroup = bgroup;
    }

private:
    int m_Number;
    int m_Option;
    bool m_Checked;
    QCheckBox* m_ButtonGroup;
};

class CustomComboBox : public QComboBox
{
public:
    CustomComboBox();
    void setParamName(const char* ParamName) {
        m_ParamName = ParamName;
    }
    void setParamType(m64p_type ParamType) {
        m_ParamType = ParamType;
    }
    void setConfigHandle(m64p_handle CurrentHandle) {
        m_CurrentHandle = CurrentHandle;
    }
    void setAuto(bool* pAuto) {
        m_Auto = pAuto;
    }

private:
    m64p_type m_ParamType;
    std::string m_ParamName;
    m64p_handle m_CurrentHandle;
    bool* m_Auto;
};

class CustomPushButton : public QPushButton
{
public:
    CustomPushButton();
    void setParamName(const char* ParamName) {
        m_ParamName = ParamName;
    }
    void setParamType(m64p_type ParamType) {
        m_ParamType = ParamType;
    }
    void setConfigHandle(m64p_handle CurrentHandle) {
        m_CurrentHandle = CurrentHandle;
    }
    void setAuto(bool* pAuto) {
        m_Auto = pAuto;
    }
    void setJoystick (int joystick) {
        m_Joystick = joystick;
    }

private:
    m64p_type m_ParamType;
    std::string m_ParamName;
    m64p_handle m_CurrentHandle;
    bool* m_Auto;
    int m_Joystick;
};

class CustomPushButton2 : public QPushButton
{
public:
    CustomPushButton2();
    void setParamName(const char* ParamName) {
        m_ParamName = ParamName;
    }
    void setParamType(m64p_type ParamType) {
        m_ParamType = ParamType;
    }
    void setConfigHandle(m64p_handle CurrentHandle) {
        m_CurrentHandle = CurrentHandle;
    }

private:
    m64p_type m_ParamType;
    std::string m_ParamName;
    m64p_handle m_CurrentHandle;
};

#endif // CUSTOMLINEEDIT_H
