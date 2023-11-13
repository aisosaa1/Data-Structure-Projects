// Test File
// Aisosa Aimufua

#include "satnet.cpp"
// #include "satnet.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
 
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester{
    public:
    double sampleTimeMeasurement(SatNet & aNet, int tempArray[], int arraySize);
    // Tester functions
    bool insertNormal();
    bool insertEdge();
    bool insertError();
    bool balance_large_insertion();
    bool removeNormal();
    bool removeEdge();
    bool balance_large_removal();
    bool remove_deorbited_normal();
    bool count_sat_normal();
    bool findSatnormal();
    bool findSaterror();
    bool assignNormal();
    bool assignError();
    bool remove_logn_Runtime();
    bool insert_logn_Runtime();
};
int main(){
    Tester test;

    cout << "*Each test for removal or insertion will have a corresponding call to check for their avl/bst properties at the end of them* \n" << endl;
    cout << "Testing insertion normal case: \n" << endl;
    if (test.insertNormal())
    {
        cout << "Test for normal insertion passed" << endl;
        cout << "//Bst and Avl Properties Maintained//\n" << endl;
    }
    else{
        cout << "Test for normal insertion failed " << endl;
        cout << "//Bst and Avl Properties Not Maintained//\n" << endl;
    }

    cout << "Testing insertion edge case: \n" << endl;
    if (test.insertEdge())
    {
        cout << "Test for edge insertion passed " << endl;
        cout << "//Bst and Avl Properties Maintained//\n" << endl;
    }
    else{
        cout << "Test for edge insertion failed " << endl;
        cout << "//Bst and Avl Properties Not Maintained//\n" << endl;
    }
    cout << "Testing insertion error case: \n" << endl;
    if (test.insertError())
    {
        cout << "Test for error insertion passed " << endl;
        cout << "//Bst and Avl Properties Maintained//\n" << endl;
    }
    else{
        cout << "Test for error insertion failed " << endl;
        cout << "//Bst and Avl Properties Not Maintained//\n" << endl;
    }

    cout << "Testing Removal normal case: \n" << endl;
    if (test.removeNormal())
    {
        cout << "Test for normal Removal passed " << endl;
        cout << "//Bst and Avl Properties Maintained//\n" << endl;
    }
    else{
    cout << "Test for normal Removal failed " << endl;
    cout << "//Bst and Avl Properties Not Maintained//\n" << endl;
    }
    cout << "Testing Removal edge case: \n" << endl;
    if (test.removeEdge())
    {
        cout << "Test for edge Removal passed " << endl;
        cout << "//Bst and Avl Properties Maintained//\n" << endl;
    }
    else{
        cout << "Test for edge Removal failed " << endl;
        cout << "//Bst and Avl Properties Not Maintained//\n" << endl;
    }
    cout << "Testing Removal Deorbited case: \n" << endl;
    if (test.remove_deorbited_normal())
    {
        cout << "Test for Deorbited Removal passed " << endl;
        cout << "//Bst and Avl Properties Maintained//\n" << endl;
    }
    else{
        cout << "Test for Deorbited Removal failed " << endl;
        cout << "//Bst and Avl Properties Not Maintained//\n" << endl;
    }
    cout << "Testing Find Sat normal case: \n" << endl;
    if (test.findSatnormal())
    {
        cout << "Test for normal Find Sat passed\n " << endl;
    }
    else{
        cout << "Test for normal Find Sat failed " << endl;
    }
    cout << "Testing Find Sat Error case: \n" << endl;
    if (test.findSaterror())
    {
        cout << "Test for Error Find Sat passed \n" << endl;
    }
    else{
        cout << "Test for Error Find Sat failed " << endl;
    }
    
    cout << "Testing Count Sat normal case: \n" << endl;
    if (test.count_sat_normal())
    {
        cout << "Test for Count Sat passed \n" << endl;
    }
    else{
        cout << "Test for Count Sat failed " << endl;
    }

    cout << "Testing normal assignment: \n" << endl;
    if (test.assignNormal())
    {
        cout << "Test for normal assignment passed \n" << endl;
    }
    else{
        cout << "Test for normal assignment failed " << endl;
    }

    cout << "Testing Error assignment: \n" << endl;
    if (test.assignError())
    {
        cout << "Test for Error assignment passed \n" << endl;
    }
    else{
        cout << "Test for Error assignment failed " << endl;
    }

    cout << "Testing Balancing Large insertion: \n" << endl;
    if (test.balance_large_insertion())
    {
        cout << "Test for Balancing Large insertion passed \n" << endl;
        cout << "//Bst and Avl Properties Maintained//\n" << endl;
    }
    else{
        cout << "Test for Balancing Large insertion failed " << endl;
        cout << "//Bst and Avl Properties Not Maintained//\n" << endl;

    }

    cout << "Testing Balancing Large Removal: \n" << endl;
    if (test.balance_large_removal())
    {
        cout << "Test for Balancing Large Removal passed \n" << endl;
        cout << "//Bst and Avl Properties Maintained//\n" << endl;
    }
    else{
        cout << "Test for Balancing Large Removal failed " << endl;
        cout << "//Bst and Avl Properties Not Maintained//\n" << endl;
    }
    
    cout << "Testing Log(n) runtime insertion: \n" << endl;
    if (test.insert_logn_Runtime())
    {
        cout << "Test for Log(n) runtime insertion passed \n" << endl;
        cout << "//Bst and Avl Properties Maintained//\n" << endl;
    }
    else{
        cout << "Test for Log(n) runtime insertion failed " << endl;
        cout << "//Bst and Avl Properties Not Maintained//\n" << endl;
    }

    cout << "Testing Log(n) runtime removal: \n" << endl;
    if (test.remove_logn_Runtime())
    {
        cout << "Test for Log(n) runtime removal passed \n" << endl;
    }
    else{
        cout << "Test for Log(n) runtime removal failed " << endl;
    }
    return 0;
}

double Tester::sampleTimeMeasurement(SatNet & aNet, int tempArray[], int arraySize){
    double T = 0.0;//stores running times
    clock_t start, stop;//stores the clock ticks while running the program
    start = clock();
    // the algorithm to be measured
    for (int j=0;j<arraySize;j++){
        aNet.findSatellite(tempArray[j]);
    }
    stop = clock();
    T = stop - start;//number of clock ticks the algorithm took
    double measureTime = T/CLOCKS_PER_SEC;//time in seconds
    return measureTime;
}


bool Tester:: insertNormal(){
    int count = 0;
    bool result = true;
    SatNet satnet;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);     // there are 4 altitudes
    Random altGen(0,3);     // there are 4 altitudes
    int id = 0;
    int treeSize = 7;
    // loop to make the tree
    for(int i=0;i<treeSize;i++){
    id = idGen.getRandNum();
    Sat sat(id,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
    satnet.insert(sat);
    }

    // there should be 7 sats in the tree
    // if any of this is false the insertion didn't work properly
    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree it should be 7: " << count << "\n" << endl;

    result = result and count == treeSize;
    // then we check if the current tree retains its avl/bst properties
    result = result and satnet.isAvl();
    result = result and satnet.isBst();   
    // based on our comparisions if the insertion worked correctly then result should return true and thus the insertion worked!
    return result;

}


bool Tester:: insertEdge(){
    // edge case would be trying to insert the max amount of sat ids possible
    // along with the minimum number of sat ids possible
    int count = 0;
    SatNet satnet;
    bool result = true;

    // creating sat obj with max id possible
    Sat maxSat(MAXID);

    // inserting Sat with max id
    satnet.insert(maxSat);

    // creating sat obj with min id possible
    Sat minSat(MINID);

    // inserting sat with min id
    satnet.insert(minSat);

    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree it should be 2: " << count << "\n" << endl;

    // since we inserted only 2 sats this should evaluate to being true
    result = result and count == 2;

    // then we check if the current tree retains its avl/bst properties
    result = result and satnet.isAvl();
    result = result and satnet.isBst();

    return result;

}

bool Tester:: insertError(){
    // error case would be trying to insert a sat with the same id into the tree
    // along with inserting a sat with id over or under the number defined by the max/min
    int count = 0;
    SatNet satnet;
    bool result = true;
    // creating sat obj with max id possible
    Sat maxSat(MAXID);

    // inserting Sat with max id
    satnet.insert(maxSat);

    // creating sat obj with max id possible
    Sat minSat(MINID);

    // inserting sat with min id
    satnet.insert(minSat);

    // creating sat obj with duplicate sat val
    Sat duplicateSat(MAXID);

    // inserting sat with duplicate id
    satnet.insert(duplicateSat);

    // creating sat obj with more max id possible
    Sat overMax(100000);

    // inserting Sat with more than max id
    satnet.insert(overMax);

    // creating sat obj with less min id possible
    Sat underMin(9999);

    // inserting sat with min id
    satnet.insert(underMin);

    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree it should be 2 (because only 2 of those values we tried to insert shouldve been valid): " << count << "\n" << endl;

    // since only 2 of those values we tried to insert shouldve been valid we should only have 2 sats in the tree so this should evaluate to being true
    result = result and count == 2;

    // then we check if the current tree retains its avl/bst properties
    result = result and satnet.isAvl();
    result = result and satnet.isBst();

    return result;
}


bool Tester:: removeNormal(){
    int count = 0;
    bool result = true;
    SatNet satnet;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);     // there are 4 altitudes
    Random altGen(0,3);     // there are 4 altitudes
    int id = 0;
    int tempID = 0;
    int treeSize = 10;
    // loop to make the tree
    for(int i=0;i<treeSize;i++){
        if (i == treeSize / 2) tempID = id;//we store this ID for later use
        id = idGen.getRandNum();
        Sat sat(id,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        satnet.insert(sat);
    }
    cout << "List of satellites after inserting " << treeSize << " nodes:\n";
    satnet.listSatellites();
    cout << endl;

    satnet.remove(tempID);
    cout << "List of satellites after removing the node with ID: " << tempID << "\n";
    satnet.listSatellites();
    cout << endl;

    // there should be 10 sats in the tree
    // if any of this is false the insertion didn't work properly
    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree it should be 9: " << count << "\n" << endl;

    result = result and count == treeSize -1;
    // then we check if the current tree retains its avl/bst properties
    result = result and satnet.isAvl();
    result = result and satnet.isBst();    
    // based on our comparisions if the insertion worked correctly then result should return true and thus the insertion worked!
    return result;
}


bool Tester::removeEdge(){
    int count = 0;
    int rand = 0;
    bool result = true;
    SatNet satnet;

    // vector to store ids of inserted satellites
    vector<int> insertedIDs;

    // Insert satellite with max possible ID and store its ID
    Sat maxSat(MAXID);
    satnet.insert(maxSat);
    insertedIDs.push_back(MAXID);

    // Insert satellite with min possible ID and store its ID
    Sat minSat(MINID);
    satnet.insert(minSat);
    insertedIDs.push_back(MINID);

    rand = MINID + 5;
    Sat randomSat(rand);
    satnet.insert(randomSat);

    insertedIDs.push_back(rand);

    cout << "List of satellites after inserting 3 nodes:\n";
    satnet.listSatellites();
    cout << endl;

    // Loop through the inserted IDs and remove them
    for(unsigned int i=0;i< insertedIDs.size();i++){
        satnet.remove(insertedIDs.at(i));
    }


    cout << "List of satellites after removing the nodes:\n";
    satnet.listSatellites();
    cout << endl;

    // Check if the tree is empty
    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree it should be 0: " << count << "\n" << endl;

    result = result and (count == 0);
    result = result and satnet.isAvl();
    result = result and satnet.isBst();
    // based on our comparisions if the removal worked correctly then result should return true and thus the removal worked!
    return result;

}

bool Tester::remove_deorbited_normal(){
    Random idGen(MINID,MAXID);
    Random stateGen(0,3);  // there are 4 inclination
    Random altGen(0,3);     // there are 4 altitudes
    Random inclinGen(0,3);
    SatNet satnet;
    bool result = true;
    int count = 0;
    int id = 0;
    int treeSize = 5;
    // loop to make the tree
    for(int i=0;i<treeSize;i++){
        id = idGen.getRandNum();
        Sat sat(id,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        sat.setState(DEORBITED);
        satnet.insert(sat);
    }
    cout << "List of satellites after inserting " << treeSize << " sats:\n";
    satnet.listSatellites();
    cout << endl;

    for(int i=0;i<treeSize;i++){
        satnet.removeDeorbited();
    }
    cout << "List of satellites after removing the sats with state Deorbited: " << "\n";
    satnet.listSatellites();
    cout << endl;

    // there should be 10 sats in the tree
    // if any of this is false the insertion didn't work properly
    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree it should be 0: " << count << "\n" << endl;

    result = result and count == 0;
    // then we check if the current tree retains its avl/bst properties
    result = result and satnet.isAvl();
    result = result and satnet.isBst();    
    // based on our comparisions if the insertion worked correctly then result should return true and thus the insertion worked!
    return result;

}

bool Tester::findSatnormal(){
    bool result = true;
    SatNet satnet;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);     // there are 4 altitudes
    Random altGen(0,3);     // there are 4 altitudes
    int id = 0;
    int tempID = 0;
    int treeSize = 10;
    // loop to make the tree
    for(int i=0;i<treeSize;i++){
        if (i == treeSize / 2) tempID = id;//we store this ID for later use
        id = idGen.getRandNum();
        Sat sat(id,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        satnet.insert(sat);
    }
    cout << "List of satellites after inserting " << treeSize << " nodes:\n";
    satnet.listSatellites();
    cout << endl;

    cout << "Checking to see if we find the sat with ID: " << tempID << "\n";
    satnet.findSatellite(tempID);
    if (satnet.findSatellite(tempID))
        cout << "Sat found in the tree" << endl;
    cout << endl;

    result = result and satnet.findSatellite(tempID);
    return result;
}

bool Tester::findSaterror(){
    // an error case would be trying to find a sat with id that doesnt exist in the tree
    bool result = true;
    SatNet satnet;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);     // there are 4 altitudes
    Random altGen(0,3);     // there are 4 altitudes
    int id = 0;
    int treeSize = 10;
    // loop to make the tree
    for(int i=0;i<treeSize;i++){
        id = idGen.getRandNum();
        Sat sat(id,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
        satnet.insert(sat);
    }
    cout << "List of satellites after inserting " << treeSize << " nodes:\n";
    satnet.listSatellites();
    cout << endl;

    cout << "Checking to see if we can find a sat with ID 1: " << "\n";
    satnet.findSatellite(1);
    if (!satnet.findSatellite(1))
        cout << "Sat not found in the tree!" << endl;
    cout << endl;

    result = result and !satnet.findSatellite(1);
    return result;
}

bool Tester::count_sat_normal(){
    int count = 0;
    bool result = true;
    SatNet satnet;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);     // there are 4 altitudes
    Random altGen(0,3);     // there are 4 altitudes
    int id = 0;
    int treeSize = 4;
    // loop to make the tree
    for(int i=0;i<treeSize;i++){
    id = idGen.getRandNum();
    Sat sat(id,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()));
    satnet.insert(sat);
    }

    // there should be 4 sats in the tree
    // if any of this is false the insertion didn't work properly
    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree it should be 4: " << count << "\n" << endl;
    satnet.listSatellites();

    result = result and count == treeSize;
    // then we check if the current tree retains its avl/bst properties
    result = result and satnet.isAvl();
    result = result and satnet.isBst();   
    // based on our comparisions if the insertion worked correctly then result should return true and thus the insertion worked!
    return result;

}

bool Tester::assignNormal(){
    bool result = true;
    int count = 0;
    // create two tree with different amounts of nodes in them
    // then try setting them equal to each other
    Sat lhSat(MINID + 1),lhSat2(MINID + 2),lhSat3(MINID + 3),rhSat(MAXID - 1),rhSat2(MAXID - 2);
    SatNet leftSide,rightSide;
    // insert them into the different trees
    leftSide.insert(lhSat);
    leftSide.insert(lhSat2);
    leftSide.insert(lhSat3);
    rightSide.insert(rhSat);
    rightSide.insert(rhSat2);

    // list out the different values in each tree before assignment
    cout << "List of satellites before assignment and after inserting into tree 1 "<<"\n";
    leftSide.listSatellites();
    cout << "\n";
    cout << "List of satellites before assignment and after inserting into tree 2 "<<"\n";
    rightSide.listSatellites();
    cout << "\n";

    cout << "this is the lefthand side now after assingment\n" << endl;
    leftSide = rightSide;
    leftSide.listSatellites();
    cout << "\n";
    count = leftSide.countSatellites(I48) + leftSide.countSatellites(I53) + leftSide.countSatellites(I70) + leftSide.countSatellites(I97);

    result = result and count == 2;
    result = result and leftSide.isAvl();
    result = result and leftSide.isBst();   
    // based on our comparisions if the insertion worked correctly then result should return true and thus the insertion worked!
    return result;


}

bool Tester::assignError(){
    // assigning an empty object to an empty object would be an example of an error case
    bool result = true;
    int count = 0;
    // create two empty trees with different amounts of nodes in them
    SatNet leftSide,rightSide;

    cout << "this is the lefthand side now after empty assingment\n" << endl;
    leftSide = rightSide;
    leftSide.listSatellites();
    cout << "\n";
    count = leftSide.countSatellites(I48) + leftSide.countSatellites(I53) + leftSide.countSatellites(I70) + leftSide.countSatellites(I97);

    result = result and count == 0;
    result = result and leftSide.isAvl();
    result = result and leftSide.isBst();   
    // based on our comparisions if the insertion worked correctly then result should return true and thus the insertion worked!
    return result;
}

bool Tester:: balance_large_insertion(){
    // insert 300 sats and chceck if the tree is still balanced
    int count = 0;
    bool result = true;
    SatNet satnet;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);     // there are 4 altitudes
    Random altGen(0,3);     // there are 4 altitudes
    int id = 0;
    int treeSize = 300;
    // loop to make the tree
    for(int i=0;i<treeSize;i++){
        id = idGen.getRandNum();
        Sat sat;
        sat.setID(id);
        satnet.insert(sat);
    }

    // there should be 300 sats in the tree
    // if any of this is false the insertion didn't work properly
    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree it should be 300: " << count << "\n" << endl;

    result = result and count == treeSize;
    // then we check if the current tree retains its avl/bst properties
    result = result and satnet.isAvl();
    result = result and satnet.isBst();   
    // based on our comparisions if the insertion worked correctly then result should return true and thus the insertion worked!
    return result;

}

bool Tester::balance_large_removal(){
    // insert 300 sats and chceck if the tree is still balanced after removing 150
    int count = 0;
    bool result = true;
    SatNet satnet;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);     // there are 4 altitudes
    Random altGen(0,3);     // there are 4 altitudes
    Random stateGen(0,2);       
    int id = 0;
    int treeSize = 300;
    // loop to make the tree
    for(int i=0;i<treeSize;i++){
        id = idGen.getRandNum();
        Sat sat(id,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()),static_cast<STATE>(stateGen.getRandNum()));
        satnet.insert(sat);
    }

    // there should be 300 sats in the tree
    // if any of this is false the insertion didn't work properly
    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree it should be 300: " << count << "\n" << endl;

    satnet.removeState(ACTIVE);
    satnet.removeState(DEORBITED);
    satnet.removeState(DECAYING);
    satnet.removeState(ACTIVE);
    
    count = satnet.countSatellites(I48) + satnet.countSatellites(I53) + satnet.countSatellites(I70) + satnet.countSatellites(I97);
    cout << "this is the total number of sats in the tree after removing 162 sats (more than 150) sats it should be 138: " << count << "\n" << endl;
    result = result and count == 138;
    // then we check if the current tree retains its avl/bst properties
    result = result and satnet.isAvl();
    result = result and satnet.isBst();   
    // based on our comparisions if the insertion worked correctly then result should return true and thus the insertion worked!
    return result;
}

bool Tester::insert_logn_Runtime() {
    SatNet satnet, satnet2; // satnet objs that were going to insert into
    double time = 0.0;

    // Time measurement for first tree (n = 1000)
    clock_t start, stop;
    start = clock();
    // The nested loop serves to repeat the insertion process multiple times in order to get a more accurate average time measurement
    // In situations where the operation is very fast, the execution time might be too short to measure accurately with certain timers.
    // for (int r = 0; r < repeatCount; r++) {
        for (int i = 0; i < 1000; i++) {
            Sat sat(i);
            satnet.insert(sat);
        }
    stop = clock();
    time = (stop - start);
    double treeOneTicks = time/CLOCKS_PER_SEC;

    // Time measurement for second tree (n = 2000)
    clock_t start2, stop2;
    start2 = clock();
        for (int i = 0; i < 2000; i++) {
            Sat sat(i);
            satnet2.insert(sat);
        }

    stop2 = clock();
    time = (stop2 - start2);
    double treeTwoTicks = time/CLOCKS_PER_SEC;

    // Calculate the ratio
    double avg = treeTwoTicks / (2 * treeOneTicks);

    // error bounds
    double bigO = 1.1 + 0.4;
    double bigOmega = 1.1 - 0.4;

    cout << "This is the ratio: " << avg << " | Lower bound: " << bigOmega << " | Upper bound: " << bigO << endl;

    if (avg <= bigO) {
        cout << "Acceptable ratio/Absolute error \n" << endl;
        return true;
    } 
    else {
        cout << "Unacceptable ratio/Absolute error \n" << endl;
        return false;
    }
}

bool Tester::remove_logn_Runtime(){
    SatNet satnet, satnet2;
    double time = 0.0;
    Random idGen(MINID,MAXID);
    Random inclinGen(0,3);
    Random altGen(0,3);
    Random stateGen(0,2);   
    Random typeGen(0,2);
    int id = 0;
    int tempIDs[2007] = {0}; // Increased size to accommodate the second tree

    // Insertion for the purpose of later removal
    for(int i=0; i<2000; i++){
        id = idGen.getRandNum();
        tempIDs[i] = id;
        Sat sat(id,static_cast<ALT>(altGen.getRandNum()),static_cast<INCLIN>(inclinGen.getRandNum()),static_cast<STATE>(stateGen.getRandNum()));
        if (i < 1000) {
            satnet.insert(sat);
        } else {
            satnet2.insert(sat);
        }
    }

    // Time measurement for first tree (n = 1000)
    clock_t start, stop;
    start = clock();
    for (int i = 0; i < 1000; i++) {
        satnet.remove(tempIDs[i]);
    }
    stop = clock();
    time = (stop - start);
    double treeOneTicks = time/CLOCKS_PER_SEC;

    // Time measurement for second tree (n = 2000)
    clock_t start2, stop2;
    start2 = clock();
    for (int i = 1000; i < 2000; i++) {
        satnet2.remove(tempIDs[i]);
    }
    stop2 = clock();
    time = (stop2 - start2);
    double treeTwoTicks = time/CLOCKS_PER_SEC;

    // Calculate the ratio
    double avg = treeTwoTicks / (2 * treeOneTicks);

    // error bounds
    double bigO = 1.1 + 0.4;
    double bigOmega = 1.1 - 0.4;

    cout << "This is the ratio: " << avg << " | Lower bound: " << bigOmega << " | Upper bound: " << bigO << endl;

    if (avg <= bigO) {
        cout << "Acceptable ratio/Absolute error \n" << endl;
        return true;
    } 
    else {
        cout << "Unacceptable ratio/Absolute error \n" << endl;
        return false;
    }
}


