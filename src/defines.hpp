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

// Holds important information about each element. 
struct Element
{
    const Name name;
    float neutrons;
    char periodicBlock;
};

#define HYDROGEN Element(Name::HYDROGEN, 0, 'S')
#define HELIUM Element(Name::HELIUM, 2, 'S')

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
    vector<Atom> atoms;     // vector of atoms in molecule

    string name = "";

    // Constructor class. Enter the first atom of the molecule, will tell child who parent, will tell parent who child.
    Molecule(Atom firstAtom)
    {
        firstAtom.parent = this;
        atoms.push_back(firstAtom);
    }
};

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
        for (int i =0; atom2 -> parent -> atoms.size(); i++)
        {
            // change the parent from atom 2' parent molecule to atom 1's parent molecule
            temp_atom2_parent -> atoms[i].parent = atom1 -> parent;
        }

        // append vector of atoms from molecule 2 to end of moelcule 1
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


// contains information on what is needed for a mechanism
class Mechanism
{
public:

};

// contains information any reaction may need, plus methods for reactions - reactions are made up of mechanisms
class Reaction
{
public: 
    
    // virtual allows child classes to be used wherever their parent class is mentioned instead
    virtual Molecule react(Molecule mol1, Molecule mol2) {} // tells compiler react will be a method in all future subclasses
};