#pragma once

#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Dataset;
using spDataset = oxygine::intrusive_ptr<Dataset>;

class Dataset : public oxygine::ref_counter
{
public:
    enum class Datatype
    {
        TRAIN,
        TEST
    };

    Dataset(QString filename);
    ~Dataset();

    const QVector<QVector<double>>& getIns(Datatype d) const;

    const QVector<QVector<double>>& getOuts(Datatype d) const;

    void split(double ptrain);

private:
    QVector<QVector<double>> m_ins;
    QVector<QVector<double>> m_outs;

    QVector<QVector<double>> m_train_ins;
    QVector<QVector<double>> m_train_outs;

    QVector<QVector<double>> m_test_ins;
    QVector<QVector<double>> m_test_outs;
};

