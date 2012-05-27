#include "complexe.h"
#include <QTextStream>

Constante& Complexe::GetVal()const
{
    Complexe c(pRe, pIm);
    return c;
}

Constante& Complexe::operator+(const Constante& c)const
{
    const Complexe* co = dynamic_cast<const Complexe*>(&c);
    if(co!=0)
    {
        Complexe res(pRe + co->GetRe(), pIm + co->GetIm());
        return res;
    }
    else
    {
        throw CalcException("L'op�ration d'addition n�cessite que les deux op�rateurs soient de m�me type");
    }
}

Constante& Complexe::operator-(const Constante& c)const
{
    const Complexe* co = dynamic_cast<const Complexe*>(&c);
    if(co!=0)
    {
        Complexe res(pRe - co->GetRe(), pIm - co->GetIm());
        return res;
    }
    else
    {
        throw CalcException("L'op�ration de soustraction n�cessite que les deux op�rateurs soient de m�me type");
    }
}

Constante& Complexe::operator*(const Constante& c)const
{
    const Complexe* co = dynamic_cast<const Complexe*>(&c);
    if(co!=0)
    {
        Complexe res(pRe*co->GetRe()-pIm*co->GetIm(), pRe*co->GetIm()+pIm*co->GetRe());
        return res;
    }
    else
    {
        throw CalcException("L'op�ration de multiplication n�cessite que les deux op�rateurs soient de m�me type");
    }
}

Constante& Complexe::operator/(const Constante& c)const
{
    const Complexe* co = dynamic_cast<const Complexe*>(&c);
    if(co!=0)
    {
        Entier e(0);
        return e;
    }
    else
    {
        throw CalcException("L'op�ration de division n�cessite que les deux op�rateurs soient de m�me type");
    }
}

Constante& Complexe::operator-()const
{
    Complexe c(pRe - Entier(2)*pRe, pIm-Entier(2)*pIm);
    return c;
}

Constante& Complexe::carree()const
{
    Entier e(0);//� impl�menter
    return e;
}

Constante& Complexe::cube()const
{
    Entier e(0);//� impl�menter
    return e;
}

QString Complexe::ToQString()
{
    QString str;
    /*QTextStream tx(&str);
    tx << pRe << "$" << pIm;*/
    return str;
}
