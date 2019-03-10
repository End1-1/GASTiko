#include "c5combobox.h"

C5ComboBox::C5ComboBox(QWidget *parent) :
    QComboBox(parent)
{

}

int C5ComboBox::getTag()
{
    return fTag;
}

void C5ComboBox::setTag(int tag)
{
    fTag = tag;
}

void C5ComboBox::setIndexForValue(const QVariant &value)
{
    setCurrentIndex(findData(value));
}
