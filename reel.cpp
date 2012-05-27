#include "reel.h"
#include <QTextStream>

Constante& Reel::GetVal()const
{
    Reel r(value);
    return r;
}

Reel::operator float()const
{
    return value;
}

Constante& Reel::operator+(const Constante& c)const
{
    const Reel* r = dynamic_cast<const Reel*>(&c);
    if(r!=0)
    {
        Reel res((float)*r + value);
        return res;
    }
    else
    {
        throw CalcException("L'op�ration d'addition n�cessite que les deux op�rateurs soient de m�me type");
    }
}

Constante& Reel::operator-(const Constante& c)const
{
    const Reel* r = dynamic_cast<const Reel*>(&c);
    if(r!=0)
    {
        Reel res(value - (float)*r);
        return res;
    }
    else
    {
        throw CalcException("L'op�ration de soustraction n�cessite que les deux op�rateurs soient de m�me type");
    }
}

Constante& Reel::operator*(const Constante& c)const
{
    const Reel* r = dynamic_cast<const Reel*>(&c);
    if(r!=0)
    {
        Reel res(value * (float)*r);
        return res;
    }
    else
    {
        throw CalcException("L'op�ration de multiplication n�cessite que les deux op�rateurs soient de m�me type");
    }
}

Constante& Reel::operator/(const Constante& c)const
{
    const Reel* r = dynamic_cast<const Reel*>(&c);
    if(r!=0)
    {
        Reel res(value / (float)*r);
        return res;
    }
    else
    {
        throw CalcException("L'op�ration de division n�cessite que les deux op�rateurs soient de m�me type");
    }
}

Constante& Reel::operator-()const
{
    Reel r(-value);
    return r;
}

Constante& Reel::operator^(const Constante& c)const
{
    const Entier* e = dynamic_cast<const Entier*>(&c);
    const Reel* r = dynamic_cast<const Reel*>(&c);
    const Rationnel* f = dynamic_cast<const Rationnel*>(&c);

    if(e!=0)
    {
        Reel res(pow(value, (int)*e));
        return res;
    }
    else if(r!=0)
    {
        Reel res(pow(value, (float)*r));
        return res;
    }
    else if(f!=0)
    {
        Reel res(pow(pow(value, f->GetNum()), -f->GetDen()));
        return res;
    }
    else
    {
        throw CalcException("L'op�ration de puissance n�cessite que l'exposant soit un entier, un rationnel ou un r�el");
    }
}

Constante& Reel::sinus()const
{
    Reel res(sin(value));
    return res;
}

Constante& Reel::cosinus()const
{
    Reel res(cos(value));
    return res;
}

Constante& Reel::tangente()const
{
    Reel res(tan(value));
    return res;
}

Constante& Reel::sinush()const
{
    Reel res(sinh(value));
    return res;
}

Constante& Reel::cosinush()const
{
    Reel res(cosh(value));
    return res;
}

Constante& Reel::tangenteh()const
{
    Reel res(tanh(value));
    return res;
}

Constante& Reel::logdec()const
{
    if(value>0)
    {
        Reel res(log10(value));
        return res;
    }
    else
    {
        throw CalcException("L'op�ration de logarithme n�cessite que l'op�rateur soit positif");
    }
}

Constante& Reel::lognep()const
{
    if(value>0)
    {
        Reel res(log(value));
        return res;
    }
    else
    {
        throw CalcException("L'op�ration de logarithme n�cessite que l'op�rateur soit positif");
    }
}

Constante& Reel::inverse()const
{
    if(value!=0)
    {
        Reel res(1/value);
        return res;
    }
    else
        throw CalcException("L'op�ration d'inverse est impossible avec z�ro");
}

Constante& Reel::racine()const
{
    if(value>=0)
    {
        Reel res(sqrt(value));
        return res;
    }
    else
        throw CalcException("L'op�ration de racine carr�e n�cessite une valeur positive");
}

Constante& Reel::carree()const
{
    Reel res(pow(value,2));
    return res;
}

Constante& Reel::cube()const
{
    Reel res(pow(value,3));
    return res;
}

QString Reel::ToQString()
{
    QString str;
    QTextStream tx(&str);
    tx << value;
    return str;
}
