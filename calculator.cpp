#include "calculator.h"

unsigned long long factorial(int n)
{
    ///fix me
    unsigned long long total = n;
    for(int i = 1; i < n; i++){
        total = total * (n - i); 
    }
    return total;
}

double factExp(double x, int n)
{
    ///fix me
    double total = 1.0;
    for(int i = 1; i <= n; i++){
        total = total * (x/i);
    }
    return total;
}

double sin(double x, int nTerms)
{
    ///fix me
    double total = x;
    char sign = '-';
    for(int i = 1, n = 3; i < nTerms; i++, n+=2){
        if(sign == '-'){
            total -= factExp(x,n);
            sign = '+';
        }
        else{
            total += factExp(x,n);
            sign = '-';
        }
    }
    return total;
}

double sqrt(double x, int nTerms)
{
    ///fix me
    double next, prev = 1.0;
    for(int i = 0; i < nTerms; i++){
        next = prev - (prev * prev - x)/(2*prev);
        prev = next;
    }
    return prev;
}

double primary()	// Number or �(� Expression �)�
{
    Token t = ts.get();
    switch (t.kind)
    {
    case '(':			       // handle �(�expression �)�
    {
        double d = expression();
        t = ts.get();
        if (t.kind != ')')
            error("')' expected");


        ///Look ahead for a fact Token here before returning d
        ///don't forget to put the Token back if it is not fact
        ///fix me
        t = ts.get();
        if (t.kind == '!'){
            d = factorial(static_cast<int>(d));	
        }
        else{
            ts.putback(t);
        }

        return d;	// return the number�s value

    }

    case '-':
        return -primary();

    case number:	// rather than case '8':
    {
        ///Look ahead for a fact Token here before returning t's value
        ///don't forget to put the Token back if it is not fact
        ///fix me
        double d = t.value;
        t = ts.get();
        if (t.kind == '!'){
            d = factorial(static_cast<int>(d));	
        }
        else{
            ts.putback(t);
        }
        
        return d; // return the number�s value

    }

    ///if the token is of type sine, then
    ///call for a new primary
    ///pass that into the sin function
    ///use nTerms = 1000
    case sine:
        ///fix me
        return sin(primary(), 1000);
        


    ///if the token is of type root, then
    ///call for a new primary
    ///pass that into the sqrt function
    ///use nTerms = 1000
    case root:
        ///fix me
        return sqrt(primary(), 1000);

    default:
        error("primary expected");
        return 0;
    }
}

///The code below this line does not need to be changed at all

double expression()	// read and evaluate: 1   1+2.5   1+2+3.14  etc.
{
    double left = term(); 			// get the Term
    while (true)
    {
        Token t = ts.get();		// get the next token�
        switch (t.kind)  			// � and do the right thing with it
        {
        case '+':
            left += term();
            break;
        case '-':
            left -= term();
            break;
        default:
            ts.putback(t);

            return left;		// return the value of the expression
        }
    }
}
double term()	// exactly like expression(), but for * and  /
{
    double left = primary(); 		// get the Primary
    while (true)
    {
        Token t = ts.get();	// get the next Token
        switch (t.kind)
        {
        case '*':
            left *= primary();
            break;
        case '/':
        {
            double d = primary();
            if  (d==0)
                error("divide by zero");
            left /= d;
            break;
        }
        default:
            ts.putback(t);
            return left;	// return the value
        }
    }
}


void clean_up_mess()
{
    ts.ignore(print);
}


void calculate()
{
    while (cin)
        try
        {
            cout << prompt;
            Token t = ts.get();
            while (t.kind == print)
                t=ts.get();	// first discard all �prints�
            if (t.kind == quit)
                return;		// quit
            ts.putback(t);
            cout << result << expression() << endl;
        }
        catch (exception& e)
        {
            cerr << e.what() << endl;		// write error message
            clean_up_mess();		// <<< The tricky part!
        }
}
