#pragma once

#include <QVBoxLayout>

namespace Shift
{
    class Pane : public QVBoxLayout
    {
        Q_OBJECT

    public:
        Pane(QWidget* parent = nullptr);

    private:

    };
}