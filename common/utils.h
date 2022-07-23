#ifndef UTILS_H
#define UTILS_H

#include <QWidget>
#include <QObject>

class Utils : public QWidget
{
    Q_OBJECT
public:
    explicit Utils(QWidget *parent = nullptr);

    static void openExplorerAndSelectFile(const QString & selectFilename);

signals:

public slots:


};

#endif // UTILS_H
