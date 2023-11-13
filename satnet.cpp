/*
  auth: Aisosa Aimufua
  desc: full implemenation of the satnet class
*/
#include "satnet.h"
SatNet::SatNet(){
    // pre order - for constructing
    m_root = nullptr;
}


SatNet::~SatNet(){
    clear();
}


// Helper Functions to assist with testing
bool SatNet:: isAvl(){
    return isAvl(m_root);
}


bool SatNet:: isAvl(Sat *root){
    if (!root)
        return true;

    if (find_sat_balance(root) > 1 or find_sat_balance(root) < -1 )
            return false;

    //recursively checks the entire tree
    return isAvl(root->m_left) and isAvl(root->m_right);
}

bool SatNet:: isBst(){
    return isBst(m_root);
}


bool SatNet:: isBst(Sat *root){
    // Essentially if the tree is empty OR we've searched the entire tree and haven't return false yet we can then return true
    // (note root == nullptr here because we've search the entire tree atp)
    if (!root)

        return true; 

    // Checking for cases wherein the opposite of bst properties is true
    if (root->getLeft())
        if (root->getID() < root->getLeft()->getID())
            return false;

    // Checking for cases wherein the opposite of bst properties is true
    if (root->getRight()) 
        if (root->getID() > root->getRight()->getID())
            return false;


    //recursively checks the entire tree
    return isBst(root->m_left) and isBst(root->m_right);
}

void SatNet::insert(const Sat& satellite){
    // !findsatellite(m_id) is basically making sure that the sat we're inserting doesnt have a value that exists in the tree already
    // basically avoidng duplicate values
    if(!findSatellite(satellite.m_id) and satellite.getID() >= MINID and satellite.getID() <= MAXID)
    // (drone to insert, recursive traversal drone)
        m_root = insertPreorder(satellite, m_root);
}


void SatNet::clear(){
    postOrderclear(m_root);
}


void SatNet::postOrderclear(Sat *destructorSat){
    // post order traversal for destruction
    if (destructorSat)
    {
        // Recurssivly deleting leftside 
        postOrderclear(destructorSat->getLeft());
        // Recurssivly deleting rightside  
        postOrderclear(destructorSat->getRight());
        delete destructorSat;
        // Setting the deleted sats equal to nullptr  
        destructorSat = nullptr;
    }   
}

void SatNet::remove(int id){
    //set m_root equal to this so it saves the changes to the tree that the function makes
    m_root = inOrderRemove(id, m_root);
}

Sat* SatNet::inOrderRemove(int id, Sat *removalSat){
    //if the node doesnt exist
    if (!removalSat)
        return removalSat;
    
    // Normal bst in in order search/operations
    // if the current id is greater than the target id it must be in the left subtree
    if (removalSat->getID() > id)
        removalSat->setLeft(inOrderRemove(id, removalSat->getLeft()));
    
    // if the current id is less than the target id it must be in the right subtree
    else if (removalSat->getID() < id)
        removalSat->setRight(inOrderRemove(id, removalSat->getRight()));
    
    else {
    // if there is no left child or no child
    if (!removalSat->getLeft())
    {
        // initualise temporary pointer to save our place in the tree while we make adjustments
        // point the sat to its right child since that exists
        Sat *tempSat = removalSat->getRight();
        delete removalSat;
        removalSat = nullptr;
        return tempSat;
    }
    
    // if there is no right child or no child
    else if (!removalSat->getRight())
    {
        // initualise temporary pointer to save our place in the tree while we make adjustments
        // point the sat to its left child since that exists
        Sat *tempSat = removalSat->getLeft();
        delete removalSat;
        removalSat = nullptr;
        return tempSat;
    }

    // Now we know that the sat we want to remove has two children
    
    // finds the leftmost node in the right subtree of the node we want to remove. 
    // otherwise known as the inorder successor of removalSat which at this point is guaranteed to have either no left child or no children at all.
    Sat *tempSat = findMin(removalSat ->getRight());

    // replacing the ID of the node you want to remove (removalSat) with the ID of its inorder successor (tempSat). 
    // This effectively "removes" the node we want to delete from the tree without actually deleting it, 
    // with the tree's order and properties preserved.
    removalSat->m_id = tempSat ->m_id; 
    // After the removal, the right subtree of removalSat might change (especially if tempSat had a right child)
    // so we update the right child pointer of removalSat with the result of the recursive call.
    removalSat->setRight(inOrderRemove(tempSat->m_id, removalSat->getRight()));
    }
    
    // after we're done with our deletions we then have to update the balances/heights of sats in the tree
    // to accomadate for the changes we've made
    adjust_sat_height(removalSat);
    // if our balance doesnt meet the avl requirements after our operations we balance it
    if (find_sat_balance(removalSat) > 1 or find_sat_balance(removalSat) < -1)
        removalSat = satelliteBalance(removalSat);


    return removalSat;
}


Sat* SatNet:: findMin(Sat* minSat){
    // if the sat doesn't have a right or left child
    // then theres no min to find so we just return
    if (!minSat)
        return nullptr;
    
    if (!minSat->getLeft())
        return minSat;

    // else we want to recursivelly keep calling the function to traverse the tree
    // til we find the actual min
    return findMin(minSat->getLeft());
}



void SatNet::dumpTree() const {
    dump(m_root);
}

void SatNet::dump(Sat* satellite) const{
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}

void SatNet::listSatellites() const {
    // in order for sorting 
    inOrder_listSatellites(m_root);

}

void SatNet::inOrder_listSatellites(Sat *traversalSat) const {
    // in order for sorting 
    if (!traversalSat)
        return;
    
    inOrder_listSatellites(traversalSat->getLeft());
    cout << traversalSat->getID() << ": " << traversalSat->getStateStr() 
    << ": " << traversalSat->getInclinStr() << ": " << traversalSat->getAltStr() << "\n";
    inOrder_listSatellites(traversalSat->getRight());  
}

bool SatNet::setState(int id, STATE state){
    // if the sat being passed doesnt have a valid id return false because there nothing to set
    if (!findSatellite(id))
        return false;
    
    return inOrdersetState(id,state,m_root);
    
}

bool SatNet::inOrdersetState(int id, STATE state, Sat *stateSat){
    //if the node doesnt exist
    if (!stateSat)
        return false;
    
    // if we find the sat we're looking for
    if (stateSat->getID() == id)
    {
        stateSat->setState(state);
        return true;
    }
    
    // Normal bst in in order search/operations
    // if the current id is greater than the target id it must be in the left subtree
    if (stateSat->getID() > id)
        return inOrdersetState(id,state, stateSat->getLeft());
    
    // if the current id is less than the target id it must be in the right subtree
    else if (stateSat->getID() < id)
       return inOrdersetState(id,state, stateSat->getRight());

    // we didnt find the node
    return false;
}

void SatNet::removeDeorbited(){
    inOrderremoveDeorbited(m_root);
}

// Helper for tester function
void SatNet::removeState(STATE num){
    inOrderremoveState(m_root,num);
}

void SatNet::inOrderremoveDeorbited(Sat *orbitSat){
    // base case
    if (!orbitSat)
        return;
    
    // left tree traversal
    inOrderremoveDeorbited(orbitSat->getLeft());

    // Save the right child pointer before potentially deleting the current node
    Sat* rightChild = orbitSat->getRight(); // ensures that we have a valid pointer to the right subtree for the subsequent traversal, even if orbitSat has been deleted.
    

    // if we find the sat we're looking for
    if (orbitSat->getState() == DEORBITED)
        remove(orbitSat->getID());
    
    // right tree traversal
    inOrderremoveDeorbited(rightChild);
}


void SatNet::inOrderremoveState(Sat *orbitSat, STATE state){
    if(orbitSat){
        // Traverse the left then right side
        inOrderremoveState(orbitSat->getLeft(), state);
        inOrderremoveState(orbitSat->getRight(), state);

        // Then if you find sat with the state that matches the one we passed in remove it
        if(orbitSat->getState() == state){
            remove(orbitSat->getID());}
    }
}

bool SatNet::findSatellite(int id) const {
    return preOrder_findSatellite(id, m_root);
}


bool SatNet:: preOrder_findSatellite(int id, Sat *traverseSat) const{
    // Base case for recursive traversal
    if (traverseSat)
    {
        // Case that all traversals come back to when recurring
        if (traverseSat->getID() == id)
            return true;

        // Traverse left side searching for == id
        else if (preOrder_findSatellite(id, traverseSat->getLeft()))
            return true;
        
        // Traverse right side searching for == id
        else if (preOrder_findSatellite(id, traverseSat->getRight()))
            return true;
        
    }
    // If we get to this point and return false then a node with the specified id doesn't exist in the tree
    return false;
}

const SatNet & SatNet::operator=(const SatNet & rhs){
    //if the left and right side aren't the same
    if (this != &rhs){
        clear();
        m_root = postOrdercopySat(rhs.m_root);
    }
    //return dereferenced this if the objects turn out to be the same
    return *this;
    
}    

// aids in helping to create a deep copy of the tree and its nodes
Sat* SatNet:: postOrdercopySat(Sat *assignSat){
    // base case
    if (!assignSat)
        return nullptr;
    
    // allocate new memory for sats to be copied over from rhs
    Sat *copySat = new Sat(assignSat->m_id,assignSat->m_altitude,assignSat->m_inclin,assignSat->m_state);

    // recursively copy the left and right subtrees of the rhs
    copySat->setLeft(postOrdercopySat(assignSat->getLeft()));
    copySat->setRight(postOrdercopySat(assignSat->getRight()));

    // after we copy we make sure to adjust the heights of each sat copied in order to maintain balance
    adjust_sat_height(assignSat);
    // return the pointer to the new tree
    return copySat;  
}

int SatNet::countSatellites(INCLIN degree) const{
   return preOrder_sat_count(degree, m_root);
}

int SatNet::preOrder_sat_count(INCLIN degree, Sat *satCounter) const{
    int satCount = 0;
    if (satCounter)
    {
        // Do a recursive preorder traveral through the tree and incriment the number of satelites with the specific incline
        satCount = satCount + preOrder_sat_count(degree, satCounter->getLeft());
        satCount = satCount + preOrder_sat_count(degree, satCounter->getRight());
        if (satCounter->getInclin() == degree)
        {
            // Incriment the satellite counter
            satCount++;
        }
        return satCount;
    }
    return satCount;
}

int SatNet:: find_sat_height(Sat *satHeight){
//returns height of indivudual sat being passed in
    if (!satHeight)
        return -1;
    
    return satHeight -> getHeight();
}


void SatNet::adjust_sat_height(Sat *satellite){
    // after we've found the height of the satilite we can then use
    // this function to adjust its place in the tree itself

    // first we check if the satellite exists if it doesnt we just return (because theres nothing to adjust)
    if(!satellite) 
        return;

    // Then we check if the satellite has no children (they equal nullptr) the height of the node is automatically = 0
    if(!satellite->m_right and !satellite->m_left)
    {
        satellite->setHeight(0);
        return;
    }

    //if setellite/node didn't have a left child but has a right child add 1 to the rights height
    if(!satellite->m_left)
    {
        satellite->setHeight(1 + satellite->m_right->getHeight());
        return;
    }

    //if setellite/node didn't have a right child but has a left child add 1 to the rights height
    if(!satellite->m_right)
    {
        satellite->setHeight(1 + satellite->m_left->getHeight());
        return;
    }

    // Then if we make it past all those conditions we know that the satellite has both children
    // and from there we can finally compare them
    if(satellite->m_left->getHeight() > satellite->m_right->getHeight()) // if the height of the left is greater add one to the height of the left
        satellite->setHeight(1 + satellite->m_left->getHeight()); 

    else //when the left child's height is less than or equal to the right child's height
        satellite->setHeight(1 + satellite->m_right->getHeight());

}


// finds the balance factor of any two sats in the bst
int SatNet:: find_sat_balance(Sat* satellite ){
    if (!satellite)
        return -1;

    return ((find_sat_height(satellite->m_left))) - (find_sat_height(satellite->m_right));   
}



//checks and balances any sat thats passed into it
Sat* SatNet::satelliteBalance(Sat *balanceSat){
    // if the balance is > 1 then the tree is left heavy
    // and we much balance accordingly
    // The outer balance check determines if the tree is left or right heavy
    if (find_sat_balance(balanceSat) > 1)
    {
        // the inner balance checks should determine the heaviness of the child nodes. 
        if (find_sat_balance(balanceSat->m_left) >= 0)
        {   
            // if the balance of the inner sat is greater than or equal to  0
            // then it only requires one balance operation
            return rotateRight(balanceSat);
        }
        // else if the tree is disjointed (ie requires a left right rotation)
        else{
            // performing a left rotation first on the sat's left side
            balanceSat -> setLeft(rotateLeft(balanceSat -> getLeft()));
            // returning the resulting avl after performing the right rotation operation
            return rotateRight(balanceSat);
        }
        
    }
    
    // if the balance is < 1 (ie negative) then the tree is right heavy
    // and we much balance accordingly
    if (find_sat_balance(balanceSat) < -1)
    {
        if (find_sat_balance(balanceSat->m_right) <= 0)
        {
            // if the balance is just zero then it only requires one balance operation
            return rotateLeft(balanceSat);
        }
        // else if the tree is disjointed (ie requires a right left rotation)
        else{
            // performing a right rotation first on the sat's left side
            balanceSat -> setRight(rotateRight(balanceSat -> getRight()));
            // returning the resulting avl after performing the left rotation operation
            return rotateLeft(balanceSat);
        }
        
    }
    
    return balanceSat;
    
}


// function that handles left rotations of satellites in the tree
Sat* SatNet::rotateLeft(Sat *root){
    if (root and root->getRight())
    {
        Sat* Z = root;
        Sat* Y = Z->getRight();
        Sat* X = Y->getLeft();
        // rotating nodes
        Z -> setRight(Y->getLeft());
        Y ->setLeft(Z);
        root = Y;
        // Now we want to calculate and return the new heights of the sats
        // so we can properly maintain balance in the avl
        adjust_sat_height(X);
        adjust_sat_height(Z);
        adjust_sat_height(Y);

        // after that we return whatever the root is because that will then be the node with the largest height
        return root;
    }
    // if the root does equal nullptr(ie we're in a situation wherein theres nothing to rotate)
    // we just return nullptr
    return nullptr;
}


// function that handles right rotations of satellites in the tree
Sat* SatNet::rotateRight(Sat *root){
    if (root and root->getLeft())
    {
        Sat* Z = root;
        Sat* Y = Z->getLeft();
        Sat* X = Y->getRight();
        // rotating nodes
        Z -> setLeft(Y->getRight());
        Y ->setRight(Z);
        root = Y;
        // Now we want to calculate and return the new heights of the sats
        // so we can properly maintain balance in the avl
        adjust_sat_height(X);
        adjust_sat_height(Z);
        adjust_sat_height(Y);

        // after that we return whatever the root is because that will then be the node with the largest height
        return root;
    }
    // if the root does equal nullptr(ie we're in a situation wherein theres nothing to rotate)
    // we just return nullptr
    return nullptr;
}

Sat* SatNet:: insertPreorder(const Sat& insertion_sat , Sat *mainSat){
    // recurssively inserts nodes in order into the tree
    if (!mainSat) //main sat in this case is m_root
    {
        // allocate memory to make a new satellite(in this case itd be the first one in the tree)
        Sat *newSatilite = new Sat(insertion_sat.getID(),insertion_sat.getAlt(),
        insertion_sat.getInclin(),insertion_sat.getState());
        mainSat = newSatilite; // adding new satellite to tree
    }
    // bst insertion ordering operations
    // if value is less move it to the left
    if (insertion_sat.getID()< mainSat->getID())
        mainSat -> setLeft(insertPreorder(insertion_sat, mainSat -> getLeft()));

    // if value is greater move it to the right
    else if (insertion_sat.getID() > mainSat->getID())
        mainSat -> setRight(insertPreorder(insertion_sat, mainSat -> getRight()));

    // after we've inserted now we want to check the heights of the nodes/sats
    // find what their heights should be then update them accordingly so we can maintain balance
    adjust_sat_height(mainSat);

    if (find_sat_balance(mainSat) < -1 or find_sat_balance(mainSat) > 1)
        mainSat = satelliteBalance(mainSat);
    // after finding the balance of the new node we return it because the function expects a sat*
    //  return type
    return mainSat;
}