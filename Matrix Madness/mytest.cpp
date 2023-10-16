// UMBC - CMSC 341 - Fall 2023 - Proj1
// Test File
// Aisosa Aimufua (mw47977)
#include "csr.cpp"
#include <iostream>
using namespace std;
class Tester{
    public:
    // CSR Tests
    bool compressNormal(CSR & pressObj,int m, int n, int array[], int arraySize);
    bool compressLessthanMatrix();
    bool compressMorethanMatrix();
    bool compress0x0Data();
    bool equalityTestNor();
    bool compressWithoutData();
    bool equalityTestEmpty();
    bool getAtExceptionindex();

    // CSR List Tests
    bool normalAssignment(CSRList &lhs, CSRList &rhs);
    bool emptyAssignment(CSRList &lhs, CSRList &rhs);
    bool getAtExceptionemptyMatrix(CSRList &list);
    bool getAtnormal(CSRList &list);

};

int main(){

    Tester test,test2,test3,test4;
    CSR aCSR;
    CSR bCSR;
    CSRList list1, list2, list3,list4;
    int Testarr[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    test2.compressNormal(aCSR,4,6,Testarr,24);
    int Testarr2[] = {0,0,0,0,100,200,0,0,300};
    test3.compressNormal(bCSR,3,3,Testarr2,9);

    list1.insertAtHead(aCSR);
    list1.insertAtHead(bCSR);
    list4.insertAtHead(bCSR);
    cout << endl << "Dumping List1:" << endl;
    list1.dump();
    cout << endl << "Dumping List2:" << endl;
    list2.dump();
    cout << "Testing normal assignment" << endl;
    list2 = list1;
    if (test.normalAssignment(list2, list1)) {
        cout << "The assignment operator worked correctly!" << endl;
        cout << endl << "This is List2 Now:" << endl;
        list2.dump();
    } else {
        cout << "The assignment operator did not work correctly." << endl;
    }
    cout << endl << "This is list List3 before assignment:" << endl;
    list3.dump();
    cout << "Testing assigning an empty object to an object that contains data. \n" << endl;
    list3 = list2;
    if (test.normalAssignment(list3, list2)) {
        cout << "The assignment operator worked correctly for empty case!" << endl;
        cout << endl << "This is List3 Now: \n" << endl;
        list3.dump();
    } else {
        cout << "The assignment operator did not work correctly." << endl;
    }

    cout << "Testing getAt normal case \n" << endl;
    test.getAtnormal(list1);
    cout << "Testing getAt empty matrix case \n" << endl;
    test.getAtExceptionemptyMatrix(list4);
    
    cout << "Testing for when the compresess data is less than the asking matrix size. \n" << endl;
    test4.compressLessthanMatrix();
    cout << "Testing for when the compresess data is more than the asking matrix size. \n" << endl;
    test4.compressMorethanMatrix();
    cout << "Testing for when the matrix has compresess data but is 0x0 \n" << endl;
    test4.compress0x0Data();
    cout << "Testing for when the matrix has initalised row x column but no compress data \n" << endl;
    test4.compressWithoutData();
    test4.equalityTestNor();
    test4.equalityTestEmpty();
    test4.getAtExceptionindex();
    return 0;
}


bool Tester:: normalAssignment(CSRList &lhs, CSRList &rhs) {
    bool result = true;

    // Pointers to iterate through the linked lists
    CSR* lhsNode = lhs.m_head;
    CSR* rhsNode = rhs.m_head;

    while (lhsNode != nullptr and rhsNode != nullptr) {
        // From here we want to iterate both lists and compare each of their characteristics while we do it
        // to make sure that they are truly equal

        // First compare the number of non-zero values from matrices
        result = result and (lhsNode->m_nonzeros == rhsNode->m_nonzeros);
        
        // The compare the number of rows
        result = result and (lhsNode->m_m == rhsNode->m_m);
        
        // Compare the number of columns
        result = result and (lhsNode->m_n == rhsNode->m_n);

        // Compare the non-zero values
        for (int i = 0; i < lhsNode->m_nonzeros; i++) {
            result = result and (lhsNode->m_values[i] == rhsNode->m_values[i]);
        }

        // Compare the column indice
        for (int j = 0; j < lhsNode->m_nonzeros; j++) {
            result = result and (lhsNode->m_col_index[j] == rhsNode->m_col_index[j]);
        }

        // Compare the row indices
        for (int z = 0; z <= lhsNode->m_m; z++) {
            result = result and (lhsNode->m_row_index[z] == rhsNode->m_row_index[z]);
        }

        // Move to the next nodes in both lists
        lhsNode = lhsNode->m_next;
        rhsNode = rhsNode->m_next;
    }

    // Check if both lists have reached their ends, ensuring they are of the same length
    result = result and (lhsNode == nullptr and rhsNode == nullptr);

    return result;
}

bool Tester::emptyAssignment(CSRList &lhs, CSRList &rhs){
    bool result = true;

    // Pointers to iterate through the linked lists
    CSR* lhsNode = lhs.m_head;
    CSR* rhsNode = rhs.m_head;

    while (lhsNode != nullptr and rhsNode != nullptr) {
        // From here we want to iterate both lists and compare each of their characteristics while we do it
        // to make sure that they are truly equal

        // First compare the number of non-zero values from matrices
        result = result and (lhsNode->m_nonzeros == rhsNode->m_nonzeros);
        
        // The compare the number of rows
        result = result and (lhsNode->m_m == rhsNode->m_m);
        
        // Compare the number of columns
        result = result and (lhsNode->m_n == rhsNode->m_n);

        // Compare the non-zero values
        for (int i = 0; i < lhsNode->m_nonzeros; i++) {
            result = result and (lhsNode->m_values[i] == rhsNode->m_values[i]);
        }

        // Compare the column indice
        for (int j = 0; j < lhsNode->m_nonzeros; j++) {
            result = result and (lhsNode->m_col_index[j] == rhsNode->m_col_index[j]);
        }

        // Compare the row indices
        for (int z = 0; z <= lhsNode->m_m; z++) {
            result = result and (lhsNode->m_row_index[z] == rhsNode->m_row_index[z]);
        }

        // Move to the next nodes in both lists
        lhsNode = lhsNode->m_next;
        rhsNode = rhsNode->m_next;
    }

    // Check if both lists have reached their ends, ensuring they are of the same length
    result = result and (lhsNode == nullptr and rhsNode == nullptr);

    return result;
}

    bool Tester::getAtnormal(CSRList &list){
        int result = list.getAt(1,2,4);
        if (result <= 0)
        {
            cout << "getAtnormal Test Failed \n" << endl;
            return result;
        }
        else{
            cout << "getAtnormal Test Passed \n " << endl;
            cout << result << endl;
            return result;
        }
        

    }

    bool Tester:: getAtExceptionemptyMatrix(CSRList &list){
        try {
        list.getAt(5,2,2);
        } catch (const exception& err) {
        cout << "Caught exception: " << err.what() << " ; Exception successfully caught, Test passed \n" << endl;
        }
        return 0;

    }


bool Tester::compressNormal(CSR & pressObj,int m, int n, int array[], int arraySize){
    bool flag = true;
    try {
    pressObj.compress(m,n,array,arraySize);
    } catch(const exception& err) {
    cout << "Exception caught: " << err.what() << endl;
    flag = false;
    return flag;
}

// If no exception was caught, this line will be executed.
cout << "Checking all values were inserted and are in their proper places \n" << endl;
cout << "Test for normal compression Passed! \n" << endl;
return flag;
}
    

bool Tester::compressLessthanMatrix() {
    bool flag = true;
    CSR pressObj;
    int m = 3;
    int n = 3;  
    int testarr[] = {1, 2, 3, 4, 5};  
    int arrSize = sizeof(testarr) / sizeof(testarr[0]);

    try {
        pressObj.compress(m, n, testarr, arrSize);
        
        // Checking if the compressed matrix behaves as expected.
        if (pressObj.getAt(0, 2) != 3 || pressObj.getAt(2, 2) != 0) {
            flag = false;
        }
    } catch (const exception& err) {
        cout << "Exception caught for compressLessthanMatrix: " << err.what() << "\n" << endl;
        flag = false;
    }

    if (flag) {
        cout << "Test for compressLessthanMatrix Passed! \n" << endl;
    } else {
        cout << "Test for compressLessthanMatrix Failed. \n" << endl;
    }

    return flag;
}

bool Tester::compressMorethanMatrix() {
    CSR pressObj;
    int m = 2;
    int n = 2;
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    try {
        pressObj.compress(m, n, array, sizeof(array) / sizeof(array[0]));

        // Checking if the compressed matrix acts as expected.
        if (pressObj.getAt(1, 1) != 4) {
            cout << "Test for compressMorethanMatrix Failed. Incorrect values in matrix." << endl;
            return false;
        }

        cout << "Test for compressMorethanMatrix Passed! \n" << endl;
        return true;
        
    } catch (const exception& err) {
        cout << "Exception caught: " << err.what() << "\n" << endl;
        cout << "Test for compressMorethanMatrix Failed due to unexpected exception." << endl;
        return false;
    }
}


bool Tester::compress0x0Data() {
    CSR pressObj;
    int m = 0;
    int n = 0;
    int array[] = {1, 2, 3, 4, 5}; 
    pressObj.compress(m, n, array, sizeof(array) / sizeof(array[0]));

    // Here we check if the resulting CSR object is empty, then if it's empty, we return true, otherwise we return false
    if (pressObj.empty()) {
        cout << "Test for compress0x0Data Passed! \n" << endl;
        return true;
    }

    cout << "Test for compress0x0Data Failed. \n" << endl;
    return false;
}

bool Tester::compressWithoutData() {
    try {
        CSR pressObj;
        int m = 3;
        int n = 3;
        // Initialise an array with no data
        int arr[] = {};  
        pressObj.compress(m, n, arr, 0);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (pressObj.getAt(i, j) != 0) {
                    cout << "Test for compressWithoutData Failed. \n" << endl;
                    return false;
                }
            }
        }

        cout << "Test for compressWithoutData Passed! \n" << endl;
        return true;
        
    } catch (const exception& err) {
        cout << "Exception caught in compressWithoutData: " << err.what() << "\n" << endl;
        return false;
    }
}


bool Tester::equalityTestNor() {
    CSR obj1, obj2;
    int m = 3, n = 3;
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    obj1.compress(m, n, array, sizeof(array) / sizeof(array[0]));
    obj2.compress(m, n, array, sizeof(array) / sizeof(array[0]));
    
    if (obj1 == obj2) {
        cout << "Test for equalityTestNormal case Passed! \n" << endl;
        return true;
    }
    cout << "Test for equalityTestNormal case Failed. \n" << endl;
    return false;
}

bool Tester::equalityTestEmpty() {
    CSR obj1, obj2;
    if (obj1 == obj2) {
        cout << "Test for equalityTestEmpty Passed! \n" << endl;
        return true;
    }
    cout << "Test for equalityTestEmpty Failed. \n" << endl;
    return false;
}

bool Tester::getAtExceptionindex() {
    CSR obj;
    int m = 2;
    int n = 2;
    int array[] = {1, 2, 3, 4};
    obj.compress(m, n, array, sizeof(array) / sizeof(array[0]));
    
    try {
        // Trying an index that doesnt exist
        obj.getAt(3, 3);  
    } catch (const exception& err) {
        cout << "Exception caught in getAtExceptionindex: " << err.what() << "\n" << endl;
        cout << "Test for getAtExceptionindex Passed! \n" << endl;
        return true;
    }
    cout << "Test for getAtExceptionindex Failed. No exception was thrown." << endl;
    return false;
}
