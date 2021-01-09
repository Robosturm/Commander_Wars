#include <QTextStream>

#include "ai/neuralnetwork/dataset/dataset.h"
#include "coreengine/globalutils.h"

Dataset::Dataset(QString filename)
{
    QFile infile(filename);
    infile.open(QIODevice::ReadOnly);
    QTextStream stream(&infile);
    QVector<double> line;
    while (!infile.atEnd())
    {
        QString textline = stream.readLine();
        auto data = textline.split(" ");
        for (const auto & entry : data)
        {
            line.push_back(entry.toDouble());
        }
        double out = line[line.size() - 1];
        line.pop_back();
        m_ins.push_back(line);
        m_outs.push_back({out});
        line.clear();
    }
}

Dataset::~Dataset()
{
}

void Dataset::split(double ptrain)
{
    for (qint32 i = 0; i < m_ins.size(); i++)
    {
        if (GlobalUtils::randDouble(0, 1) < ptrain)
        {
            m_train_ins.append(m_ins[i]);
            m_train_outs.append(m_outs[i]);
        }
        else
        {
            m_test_ins.append(m_ins[i]);
            m_test_outs.append(m_outs[i]);
        }
    }
}


const QVector<QVector<double>>& Dataset::getIns(Datatype d) const
{ 
    if(d == Datatype::TRAIN)
    {
        return m_train_ins;
    }
    return m_test_ins;
}

const QVector<QVector<double>>& Dataset::getOuts(Datatype d) const
{
    if (d == Datatype::TRAIN)
    {
        return m_train_outs;
    }
    return m_test_outs;
}





