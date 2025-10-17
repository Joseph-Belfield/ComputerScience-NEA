#include <iostream>
#include <stack>
#include <vector>
using namespace std;

// NOTES
// variable -> function() is equal to (*varible).function()

// Element enum, ties atomic number and element name. 
enum Name
{
    HYDROGEN = 1,
    HELIUM,
    LITHIUM,
    BERYLLIUM, 
    BORON,
    PHOSPHORUS,
    CARBON,
    NITROGEN,
    OXYGEN,
    FLUORINE,
    NEON
};

/*
=========================================================================
ELEMENT, ATOM & MOLECULE CLASSES
=========================================================================
*/

// Holds important information about each element. 
struct Element
{
    const Name name;
    float neutrons;
    char periodicBlock;
};

// Atom class, holds essential details for atoms. 
struct Atom 
{ 
    const Name element;

    int neutrons;
    int electrons;

    vector<Atom*> bonds;   // vector of pointers to atoms
    Molecule* parent = nullptr;  // pointer to molecule atom is in
};

// A class for effectively anything rendered - functional groups will be a subclass
class Molecule
{
public: 
    vector<Atom*> atoms;     // vector of pointers to atoms in molecule

    string name = "";

    // Constructor class. Enter the first atom of the molecule, will tell child who parent, will tell parent who child.
    Molecule(Atom firstAtom)
    {
        firstAtom.parent = this;
        atoms.push_back(&firstAtom);
    }

    Molecule(vector<Atom*>* atomVector)
    {
        for (int i = 0; atomVector -> size(); i++)
        {
            (*atomVector)[i] -> parent = this;  // dereferences atom vector, then gets the atom i and changes its parent
            atoms.push_back((*atomVector)[i]); // adds a copy of the atom from the pointer to the vector atoms

            /* 
            may need to dereference atom pointers later in the program

            delete (*atomVector)[i];    // deletes the pointer
            (*atomVector)[i] = nullptr; // sets the pointer to null so it cannot be rereferences
            */
        }
    }
};

/*
=========================================================================
MOLECULE FUNCTIONS
=========================================================================
*/

// will join 2 molecules together at set atoms, or add another bond between atoms in a molecule 
void addBond(Atom* atom1, Atom* atom2)
{
    if (atom1 -> parent == atom2 -> parent) // if the atoms share a parent (are in the same molecule)
    {
        // add an additional bond to each others' bond vectors
        atom1 -> bonds.push_back(atom2);
        atom2 -> bonds.push_back(atom1);
    }
    else
    {
        // holds atom 2's parent molecule so its saved when atom 2 is wiped 
        Molecule* temp_atom2_parent = atom2 -> parent;

        // for each atom in atom 2's parent molecule
        for (int i = 0; atom2 -> parent -> atoms.size(); i++)
        {
            // change the parent from atom 2' parent molecule to atom 1's parent molecule
            temp_atom2_parent -> atoms.at(i) -> parent = atom1 -> parent;
        }

        // append vector of atoms from molecule 2 to end of molecule 1
        atom1 -> parent -> atoms.insert(atom1 -> parent -> atoms.end(), atom2 -> parent -> atoms.cbegin(), atom2 -> parent -> atoms.cend());

        // wipes the name of molecule 1 
        atom1 -> parent -> name = "placeholder";

        // adds a bond between atom 1 and 2
        atom1 -> bonds.push_back(atom2);
        atom2 -> bonds.push_back(atom1);

        // frees the memory which held atom 2's parent molecule
        delete temp_atom2_parent;

        // prevents other parts of the program from accessing cleared memory accidentally by removing tracks
        temp_atom2_parent = nullptr;
    }
}

int atomSearch(vector<Atom*>* atoms, Atom* targetAtom)
{
    int count = 0;
    for (int i = 0; i < atoms -> size(); i++)
    {
        if (atoms -> at(i) == targetAtom)
        {
            count++;
        }
    }

    return count;
}

// fills vector with pointers to any atom connected to atom1's tree
void findMoleculeTree(Atom* currentAtom, vector<Atom*>* moleculeVector)
{
    // checks number of times the current atom is in the molecule vector
    int atomAccountedFor = atomSearch(moleculeVector, currentAtom);

    // if the atom isn't found, add the atom - just adds parameter atom at start of function run
    if (atomAccountedFor == 0)
    {
        moleculeVector -> push_back(currentAtom);
    }

    // if that atom is found more than once, throw an error message
    else if (atomAccountedFor >= 2)
    {
        cout << "ERROR - atom found twice in molecule string" << "\n";
    }

    // for each atom bonded to current atom
    for (int i = 0; currentAtom -> bonds.size(); i++)
    {
        if (atomSearch(moleculeVector, currentAtom -> bonds[i]) == 0) // if the atom doesn't appear in the molecule list
        {
            findMoleculeTree(currentAtom -> bonds[i], moleculeVector);  // recurse the function, passing in the new branch and the molecule list as parameters
        }
    }
    
    return; // if all atoms attatched to an atom have been found, return function
}

// will remove a bond between two atoms, and if applicable, split a larger molecule into two smaller parts
Molecule* removeBond(Atom* atom1, Atom* atom2)
{
    // maintains if atom2 has been found in atom1's bonds
    bool foundA = false;

    // iterates through atom1's bonds until it finds atom2, then removes that bond
    for (int i = 0; i < atom1 -> bonds.size() && !foundA; i++)
    {   
        // if atom 2 is found in list bonds
        if (atom1 -> bonds[i] == atom2)
        {
            // remove atom 2, set found check as true
            atom1 -> bonds.erase(atom1 -> bonds.begin() + i);
            foundA = true;
        }
    }

    // repeats process above for atom2
    bool foundB = false;

    for (int i = 0; i < atom2 -> bonds.size() && !foundB; i++)
    {
        if (atom2 -> bonds[i] == atom1)
        {
            atom2 -> bonds.erase(atom2 -> bonds.begin() + i);
            foundB = true;
        }
    }

    // creates a variable delcaring if the molecule has been split by the bond breaking - defaults to true
    bool moleculeSplit = true;

    // creates a pointer to a vector of pointers to all atoms in molecule1 (molecule attatched to atom1)
    vector<Atom*>* molecule1_atomVector;
    
    // fills molecule1_atomVector with all atoms attatched to atom1
    findMoleculeTree(atom1, molecule1_atomVector);

    // checks each atom in the vector of atoms in the molecule atom1
    for (int i = 0; molecule1_atomVector -> size(); i++)
    {
        // if one of the atoms is molecule 2, the molecule has not split
        if (molecule1_atomVector -> at(i) == atom2)
        {
            moleculeSplit = false;
        }
    }

    // if the molecule hasn't split, return a null pointer, as no new molecules were made
    if (moleculeSplit == false)
    {
        return nullptr;
    } 
    else    // else return a pointer to a new molecule created off of the atoms connected to atom2 
    {
        // sets atom1's parent molecule's atom vector as the on found for atom1 post bond breaking
        atom1 -> parent -> atoms = *molecule1_atomVector;

        // creates a new molecule vector for atom2's molecule and fills it
        vector<Atom*>* molecule2_atomVector;
        findMoleculeTree(atom2, molecule2_atomVector);

        // initializes molecule2 from it's atom vector
        Molecule molecule2(molecule2_atomVector);

        // for all atoms in molecule, set molecule2 as their parent
        for (int i = 0; i < molecule2.atoms.size(); i++)
        {
            molecule2.atoms.at(i) -> parent = &molecule2;
        }

        return &molecule2;
    }
}

/*
=========================================================================
FUNCTIONAL GROUP SEARCHES
=========================================================================
*/

class Functional_Group
{
public:
    // holds pointers to the atoms 
    vector<Atom> constituantAtoms;

    // virtual allows child classes to be used wherever their parent class is mentioned instead
    // finds the first alcohol group it can in a molecule, and returns a pointer to the O
    virtual Atom* findFunctionalGroup(Molecule* molecule) {} // tells compiler react will be a method in all future subclasses

    // finds the first alcohol group it can in a molecule, and returns a pointer to a new alcohol object made of these atoms
    virtual Functional_Group* findFunctionalGroup(Molecule* molecule, bool create) {}

    // finds all alcohol groups it can in a molecule, and returns a vector of alcohol object pointers
    virtual Functional_Group* findAllFunctionalGroup(Molecule* molecule) {}
};

// alcohol is a child class of functional group
class Alcohol: public Functional_Group
{
public:
    // constiunt atoms should be stored as [O,H]

    // verifies if two given atoms are an alcohol group
    bool isAlcohol(Atom* atom1, Atom* atom2)
    {
        Atom* oxygenAtom;
        Atom* hydrogenAtom;
        if (atom1 -> element == OXYGEN && atom2 -> element == HYDROGEN)
        {
            oxygenAtom = atom1;
            hydrogenAtom = atom2;
        }
        else if (atom2 -> element == OXYGEN && atom1 -> element == HYDROGEN)
        {
            oxygenAtom = atom2;
            hydrogenAtom = atom1;
        }
        else
        {
            // if they aren't a hydrogen and oxygen, they cannot be a hydroxyl group
            return false;
        }

        // checks if the oxygen atom and hydrogen atom are bonded (hydrogen should only have 1 bond)
        bool bonded = false;
        for (int i = 0; i < oxygenAtom -> bonds.size(); i++)
        {
            if (oxygenAtom -> bonds.at(i) == hydrogenAtom && hydrogenAtom -> bonds.at(0) == oxygenAtom)
            {
                bonded = true;
            }
        }
        if (bonded == false)
        {
            return false;
        }

        for (int i = 0; i < oxygenAtom -> bonds.size(); i++)
        {
            // if the oxygen atom is bonded to a carbon
            if (oxygenAtom -> bonds.at(i) -> element == CARBON)
            {
                Atom* attatchedCarbon = oxygenAtom -> bonds.at(i);
                Atom* otherOxygen[2] = {nullptr};

                // check through all of carbons bonds for oxygen (except the given atom) - if none of the bonds are a repeated oxygen, return true
                for (int j = 0; j < attatchedCarbon -> bonds.size(); j++)
                {
                    if (attatchedCarbon -> bonds.at(j) == oxygenAtom)
                    {
                        j++;
                    }
                    else if (attatchedCarbon -> bonds.at(j) -> element == OXYGEN && otherOxygen[0] == nullptr )
                    {
                        otherOxygen[0] = attatchedCarbon -> bonds.at(j);
                    }
                    else if (attatchedCarbon -> bonds.at(j) -> element == OXYGEN && attatchedCarbon -> bonds.at(j) != otherOxygen[0] && otherOxygen[1] == nullptr)
                    {
                        otherOxygen[1] = attatchedCarbon -> bonds.at(j);
                    }
                    else if (attatchedCarbon -> bonds.at(j) -> element == OXYGEN && attatchedCarbon -> bonds.at(j) != otherOxygen[0] && attatchedCarbon -> bonds.at(j) != otherOxygen[1])
                    {
                        return true;
                    }
                }
            }
        }

        // if the above conditions are failed, it's false
        return false;
    }

    // finds the first alcohol group it can in a molecule, and returns a pointer to the O
    Atom* findAlcohol(Molecule* molecule)
    {

    }

    // finds the first alcohol group it can in a molecule, and returns a pointer to a new alcohol object made of these atoms
    Alcohol* findAlcohol(Molecule* molecule, bool create)
    {

    }

    // finds all alcohol groups it can in a molecule, and returns a vector of alcohol object pointers
    vector<Alcohol*>* findAllAlcohol(Molecule* molecule)
    {

    }

    // finds the alcohol type (void used as it does not need a parameter)
    unsigned int alcoholType(void)
    {

    }
};
/*
=========================================================================
REACTION CLASSES
=========================================================================
*/

// contains information any reaction may need, plus methods for reactions
class Reaction
{
public: 
    
    // virtual allows child classes to be used wherever their parent class is mentioned instead
    virtual Molecule react(Molecule mol1, Molecule mol2) {} // tells compiler react will be a method in all future subclasses
};

