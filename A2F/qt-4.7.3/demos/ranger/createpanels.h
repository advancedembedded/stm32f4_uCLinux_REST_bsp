#ifndef CREATEPANELS_H
#define CREATEPANELS_H

#include "rangercommon.h"

class CreatePanels : public RangerCommon
{
    Q_OBJECT

public:
    explicit CreatePanels();
    QGroupBox *blowme();

public slots:
    void blowmePressed(QAbstractButton* button);

};

#endif // CREATEPANELS_H
