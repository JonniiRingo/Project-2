int main(){
    int* p { new int { 42 } };
    smart_ptr<int> sp1 { p };

    cout << "Ref count is " << sp1.ref_count() << endl;	// Ref Count is 1
    {
       smart_ptr<int> sp2 { sp1 };
       cout << "Ref count is " << sp1.ref_count() << endl;	// Ref Count is 2
       cout << "Ref count is " << sp2.ref_count() << endl;	// Ref Count is 2
    }
    cout << "Ref count is " << sp1.ref_count() << endl;	// Ref Count is 1
    
    smart_ptr<int> sp3;

    cout << "Ref count is " << sp3.ref_count() << endl;	// Ref Count is 0
    
    sp3 = sp1;

    cout << "Ref count is " << sp1.ref_count() << endl;	// Ref Count is 2
    cout << "Ref count is " << sp3.ref_count() << endl;	// Ref Count is 2

    smart_ptr<int> sp4 { std::move(sp1) };

    cout << *sp4 << " " << *sp3 << endl;        // prints 42 42
    cout << *sp1 << endl;    
    
    return 0;                    // throws null_ptr_exception
}





// The arrow operator will only compile and work if the referred object is a class type

    struct Point { int x = 2; int y = -5; };
    
    int main ( ) {

       smart_ptr<Point> sp { new Point };
       cout << sp->x << " " << sp->y << endl;   // prints 2 -5
    }




// Here is an example of the clone member function

        smart_ptr<double> dsp1 { new double {3.14} };
    smart_ptr<double> dsp2, dsp3;
    
    dsp3 = dsp2 = dsp1;

    cout << dsp1.ref_count() << " " << dsp2.ref_count() << " " << dsp3.ref_count() << endl;
      // prints 3 3 3
    cout << *dsp1 << " " << *dsp2 << " " << *dsp3 << endl;
      // prints 3.14 3.14 3.14

    dsp1.clone();        // returns true

    cout << dsp1.ref_count() << " " << dsp2.ref_count() << " " << dsp3.ref_count() << endl;
      // prints 1 2 2
    cout << *dsp1 << " " << *dsp2 << " " << *dsp3 << endl;
      // prints 3.14 3.14 3.14