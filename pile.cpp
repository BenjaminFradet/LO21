#include "pile.h"
#include "QStringList"
#include "QRegExp"
#include "QString"

/*Mementos* Mementos::instance = 0;

Mementos& Mementos::getInstance(int dim)
{
    if(instance == 0)
        instance = new Mementos(dim);
    return *instance;
}

void Mementos::releaseInstance()
{
    if(instance != 0)
        delete instance;
    instance = 0;
}*/

Mementos::Mementos(int dim)
{
    taille = dim;
    sommet = 0;
    mementos = new Memento*[taille]();
}

Mementos::~Mementos()
{
    delete[] mementos;
}

void Mementos::Empiler(Memento* m)
{
    if(sommet == taille)
        throw CalcException("Pile pleine");
    else
        mementos[sommet++] = m;
}

Memento* Mementos::Depiler()
{
    if(sommet == 0)
        throw CalcException("Pile vide");
    else
        return mementos[--sommet];
}

Memento* Mementos::Tete()const
{
    if(sommet == 0)
        throw CalcException("Pile vide");
    else
        return mementos[sommet-1];
}

void Mementos::Clear()
{
    sommet = 0;
}

Pile* Pile::instance = 0;

Pile& Pile::getInstance(int dim)
{
    if(instance == 0)
        instance = new Pile(dim);
    return *instance;
}

void Pile::releaseInstance()
{
    if(instance != 0)
        delete instance;
    instance = 0;
}

Pile::Pile(int dim)
{
    taille = dim;
    sommet = 0;
    tabElmt = new Constante*[taille]();
    undos = new Mementos(dim);
    redos = new Mementos(dim);
}

Pile::~Pile()
{
    delete[] tabElmt;
    delete undos;
    delete redos;
}

void Pile::Empiler(Constante* c)
{
    if(sommet == taille)
        throw CalcException("Pile pleine");
    else
        tabElmt[sommet++] = c;
}

Constante* Pile::Depiler()
{
    if(sommet == 0)
        throw CalcException("Pile vide");
    else
        return tabElmt[--sommet];
}

Constante* Pile::Tete()const
{
    if(sommet == 0)
        throw CalcException("Pile vide");
    else
        return tabElmt[sommet-1];
}

void Pile::Clear()
{
    sommet = 0;
}

void Pile::Dup()
{
    if(sommet == taille)
        throw CalcException("Pile pleine");
    else
    {
        Constante* tmp = tabElmt[sommet-1];
        tabElmt[sommet++] = tmp;
    }
}

void Pile::Drop()
{
    if(sommet == 0)
        throw CalcException("Pile vide");
    else
        sommet--;
}

void Pile::createMemento()const
{
    Memento* m = new Memento(taille, sommet, tabElmt);
    undos->Empiler(m);
}

void Pile::retrieveMemento()const
{
    Memento* m = new Memento(taille, sommet, tabElmt);
    redos->Empiler(m);
}

void Pile::Annuler()
{
    Memento* m = undos->Depiler();
    retrieveMemento();
    taille = m->taille;
    sommet = m->sommet;
    tabElmt = m->tabElmt;
}

void Pile::Retablir()
{
    Memento* m = redos->Depiler();
    taille = m->taille;
    sommet = m->sommet;
    tabElmt = m->tabElmt;
}

void Pile::Plus(QString mode)
{
    ConstanteFactory* factory = new ConstanteFactory();
    if(sommet>=2)
    {
        Constante* a = this->Depiler();
        Constante* b = this->Depiler();
        createMemento();
        if(mode == "Entier")
        {
            Constante& tmp = (Entier)*b + (Entier)*a;
            Constante* c = factory->GetConstante(tmp.ToQString(), mode);
            Empiler(c);
        }
    }
    else
        throw CalcException("Cette op�ration n�cessite deux op�randes");
}

void Pile::Moins(QString mode)
{
    ConstanteFactory* factory = new ConstanteFactory();
    if(sommet>=2)
    {
        Constante* a = this->Depiler();
        Constante* b = this->Depiler();
        createMemento();
        Constante& tmp = *b - *a;
        Constante* c = factory->GetConstante(tmp.ToQString(), mode);
        Empiler(c);
    }
    else
        throw CalcException("Cette op�ration n�cessite deux op�randes");
}

void Pile::Multiplier(QString mode)
{
    ConstanteFactory* factory = new ConstanteFactory();
    if(sommet>=2)
    {
        Constante* a = this->Depiler();
        Constante* b = this->Depiler();
        createMemento();
        Constante& tmp = *b * *a;
        Constante* c = factory->GetConstante(tmp.ToQString(), mode);
        Empiler(c);
    }
    else
        throw CalcException("Cette op�ration n�cessite deux op�randes");
}

void Pile::Diviser(QString mode)
{
    ConstanteFactory* factory = new ConstanteFactory();
    if(sommet>=2)
    {
        Constante* a = this->Depiler();
        Constante* b = this->Depiler();
        createMemento();
        Constante& tmp = *b / *a;
        Constante* c = factory->GetConstante(tmp.ToQString(), mode);
        Empiler(c);
    }
    else
        throw CalcException("Cette op�ration n�cessite deux op�randes");
}

void Pile::Puissance(QString mode)
{
    ConstanteFactory* factory = new ConstanteFactory();
    if(sommet>=2)
    {
        Constante* a = this->Depiler();
        Constante* b = this->Depiler();
        createMemento();
        Constante& tmp = *b ^ *a;
        Constante* c = factory->GetConstante(tmp.ToQString(), mode);
        Empiler(c);
    }
    else
        throw CalcException("Cette op�ration n�cessite deux op�randes");
}

void Pile::Modulo(QString mode)
{
    if(sommet>=2)
    {
        Constante* a = this->Depiler();
        Constante* b = this->Depiler();
        const Entier* e1 = dynamic_cast<const Entier*>(a);
        const Entier* e2 = dynamic_cast<const Entier*>(b);
        if(e1 != NULL && e2 != NULL && mode == "Entier")
        {
            createMemento();
            Constante* c = new Entier(*b % *a);
            Empiler(c);
        }
        else
            throw CalcException("Cette op�ration n�cessite deux entiers");
    }
    else
        throw CalcException("Cette op�ration n�cessite deux op�randes");
}

void Pile::Signe(QString mode)
{
    ConstanteFactory* factory = new ConstanteFactory();
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante& tmp = -*a;
        Constante* c = factory->GetConstante(tmp.ToQString(), mode);
        Empiler(c);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Sinus()
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante* res = new Reel(a->sinus());
        Empiler(res);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Cosinus()
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante* res = new Reel(a->cosinus());
        Empiler(res);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Tangente()
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante* res = new Reel(a->tangente());
        Empiler(res);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Sinush()
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante* res = new Reel(a->sinush());
        Empiler(res);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Cosinush()
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante* res = new Reel(a->cosinush());
        Empiler(res);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Tangenteh()
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante* res = new Reel(a->tangenteh());
        Empiler(res);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::LogaNep()
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante* res = new Reel(a->lognep());
        Empiler(res);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::LogaDec()
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante* res = new Reel(a->logdec());
        Empiler(res);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Inverse(QString mode)
{
    ConstanteFactory* factory = new ConstanteFactory();
    if(sommet>=1)
    {
        Constante* a = this->Depiler();        
        createMemento();
        Constante& tmp = a->inverse();
        Constante* c = factory->GetConstante(tmp.ToQString(), mode);
        Empiler(c);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Racine()
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante* res = new Reel(a->racine());
        Empiler(res);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Carree(QString mode)
{
    ConstanteFactory* factory = new ConstanteFactory();
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante& tmp = a->carree();
        Constante* c = factory->GetConstante(tmp.ToQString(), mode);
        Empiler(c);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Cube(QString mode)
{
    ConstanteFactory* factory = new ConstanteFactory();
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        createMemento();
        Constante& tmp = a->cube();
        Constante* c = factory->GetConstante(tmp.ToQString(), mode);
        Empiler(c);
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Factorielle(QString mode)
{
    if(sommet>=1)
    {
        Constante* a = this->Depiler();
        const Entier* e = dynamic_cast<const Entier*>(a);
        if(e!=0 && mode == "Entier")
        {
            createMemento();
            Constante* res = new Entier(!(Entier)*a);
            Empiler(res);
        }
        else
            throw CalcException("Cette op�ration n�cessite un entier");
    }
    else
        throw CalcException("Cette op�ration n�cessite une op�rande");
}

void Pile::Swap(int x, int y)
{
    if(sommet>x && sommet>y)
    {
        createMemento();
        Constante* tmp;
        tmp = tabElmt[x];
        tabElmt[x] = tabElmt[y];
        tabElmt[y] = tmp;
    }
    else
        throw CalcException("Les indices doivent �tre dans la pile");
}

Constante& Pile::Sum(int x)
{
    if(x<sommet)
    {
        createMemento();
        Constante* sum;
        sum = tabElmt[sommet];
        const Entier* e = dynamic_cast<const Entier*>(sum);
        const Reel* r = dynamic_cast<const Reel*>(sum);
        const Rationnel* f = dynamic_cast<const Rationnel*>(sum);
        const Complexe* c = dynamic_cast<const Complexe*>(sum);
        if(e!=0)
        {
            for(int i = 1; i<x; i++)
            {
                const Entier* p = dynamic_cast<const Entier*>(tabElmt[sommet-i]);
                if(p!=0)
                    *sum = *sum + *tabElmt[sommet-i];
                else
                    throw CalcException("On ne peut pas faire la somme sur des constantes de type diff�rent");
            }
            return *sum;
        }
        else if(r!=0)
        {
            for(int i = 1; i<x; i++)
            {
                const Reel* p = dynamic_cast<const Reel*>(tabElmt[sommet-i]);
                if(p!=0)
                    *sum = *sum + *tabElmt[sommet-i];
                else
                    throw CalcException("On ne peut pas faire la somme sur des constantes de type diff�rent");
            }
            return *sum;
        }
        else if(f!=0)
        {
            for(int i = 1; i<x; i++)
            {
                const Rationnel* p = dynamic_cast<const Rationnel*>(tabElmt[sommet-i]);
                if(p!=0)
                    *sum = *sum + *tabElmt[sommet-i];
                else
                    throw CalcException("On ne peut pas faire la somme sur des constantes de type diff�rent");
            }
            return *sum;
        }
        else if(c!=0)
        {
            for(int i = 0; i<x; i++)
            {
                const Complexe* p = dynamic_cast<const Complexe*>(tabElmt[sommet-i]);
                if(p!=0)
                    *sum = *sum + *tabElmt[sommet-i];
                else
                    throw CalcException("On ne peut pas faire la somme sur des constantes de type diff�rent");
            }
            return *sum;
        }
    }
    else
        throw CalcException("Cet indice est trop grand par rapport au nombre d'�l�ments contenus dans la pile");
}

Constante& Pile::Mean(int x)
{
    createMemento();
    Constante& mean = Sum(x);
    const Entier* e = dynamic_cast<const Entier*>(&mean);
    const Reel* r = dynamic_cast<const Reel*>(&mean);
    const Rationnel* f = dynamic_cast<const Rationnel*>(&mean);
    const Complexe* c = dynamic_cast<const Complexe*>(&mean);

    if(e!=0)
    {
        Entier tmp(x);
        return mean / tmp;
    }
    else if(r!=0)
    {
        Reel tmp(x);
        return mean / tmp;
    }
    else if(f!=0)
    {
        Rationnel tmp(x, 1);
        return mean / tmp;
    }
    else if(c!=0)
    {
        Entier e(x);
        Complexe tmp(e, e);
        return mean / tmp;
    }
    else
        throw CalcException("Mauvais type");
}

int Pile::rowCount (const QModelIndex &parent) const
{return sommet;}

QVariant Pile::data (const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= sommet)
        return QVariant();
    if (role == Qt::DisplayRole)
       return tabElmt[index.row()]->ToQString();
    else
        return QVariant();
}


void Pile::Parser(QString s)
{
    QStringList elements = s.split(" ");
    QRegExp complexe("^[0-9]+\\${1}[0-9]+$");  // � compl�ter, complexe prend 2 constantes
    QRegExp rationnel ("^[0-9]+/{1}[0-9]+$");
    QRegExp reel ("^[0-9]+\.{1}[0-9]+$");
    QRegExp entier ("^[0-9]+$");


    for (int i = 0; i< elements.size(); i++)
    {
    /*    if(elements[i].contains(complexe))
            nouveau = &Complexe(elements[i]);*/

         if(elements[i].contains(rationnel))
         {      Constante* nouveau2 = new Rationnel(elements[i]);
              Empiler(nouveau2);
         }

        else if(elements[i].contains(reel))
         {      Constante* nouveau2 = new Reel(elements[i]);
              Empiler(nouveau2);
         }
        else if(elements[i].contains(entier))
         {      Constante* nouveau2 = new Entier(elements[i]);
              Empiler(nouveau2);
         }



        else if(elements[i] == "+")//besoin du mode
             Plus(MainWindow::getMode());//besoin du mode

        else if(elements[i] == "-")
            Moins(MainWindow::getMode());

        else if(elements[i] == "*")
            Multiplier(MainWindow::getMode());

        else if(elements[i] == "POW")
            Puissance(MainWindow::getMode());

         else if(elements[i] == "MOD")
             Modulo(MainWindow::getMode());

         else if(elements[i] == "SIGN")
            Signe(MainWindow::getMode());

         else if(elements[i] == "SIN")
             Sinus();

         else if(elements[i] == "COS")
             Cosinus();

         else if(elements[i] == "TAN")
             Tangente();

         else if(elements[i] == "COSH")
             Cosinush();

         else if(elements[i] == "SINH")
             Sinush();

         else if(elements[i] == "TANH")
             Tangenteh();

         else if(elements[i] == "LOG")
             LogaDec();

         else if(elements[i] == "LN")
             LogaNep();

         else if(elements[i] == "INV")
             Inverse(MainWindow::getMode());

         else if(elements[i] == "SQRT")
             Racine();

         else if(elements[i] == "SQR")
             Carree(MainWindow::getMode());

         else if(elements[i] == "CUBE")
             Cube(MainWindow::getMode());

         else if(elements[i] == "!")
             Factorielle(MainWindow::getMode());
    }
}
