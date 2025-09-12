#include <iostream>
#include <stack>
using namespace std;

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
class Atom 
{
public: 
    int AtomID;
    const Name element;
    float neutrons;
    int electrons;
    Atom** bonds;   // pointer to array of pointers to other atom objects

    // Constructor class. This assigns the default value of the attributes.
    Atom(const Element element) : bonds(new Atom* [element.name]),  // Assigns a pointer to an array of pointers to Atoms for bonds
                                  element(element.name),            // Assigns an enum that represents name/protons of an element
                                  neutrons(element.neutrons),       // Assigns float for default number of neutrons
                                  electrons(element.name) {};       // Assigns int for default of electrons

                                
    // Calculates difference between protons and electrons (ie: charge)
    int calculateCharge()
    {
        return (this -> element) - (this -> electrons);
    }

};

// checks if pointer is new and unique, or already present in seen atom stack
template <typename T> // allows any datatype to be in the stack (T acts as temp variable) so it can be reused
bool isInStack(stack<T> atom_stack, T value)
{
    while !(atom_stack.empty())     // while stack isn't empty
    {
        if (atom_stack.pop() == value) {return false;}  // if value matches item from stack, return false (not unique)
    }

    return true; // if value does not appear, return true (unique!)
}

// A class for effectively anything rendered - functional groups will be a subclass
class Molecule
{
private:
    const unsigned int moleculeID;
    unsigned int nextID = 0;
    unsigned int atomCounter = 0;

public: 
    string name = "";
    Atom* root = nullptr;

    // Constructor class. This assigns the default value of the attributes.
    Molecule(const unsigned int moleculeID) : moleculeID(moleculeID) {};

    // Creates a 'molecule' out of 1 atom - only run once per molecule
    void addAtom(Atom* atom)
    {
        atom -> AtomID = this -> nextID++;  // sets atom's ID to next availible ID, then increments
        root = atom;                        // sets root of molecule to atom
        atomCounter++;                      // Increases the number of atoms in molecule counter by 1
    }

    // recursively search for unseen atom
    void recursiveSearch(Atom* startAtom, stack<Atom*>* seenAtoms_stack) 
    {
        // add the start atom to be the first entry of stack of seen atoms
        seenAtoms_stack -> push(startAtom);

        for (int i = 0; i < sizeof(*startAtom -> bonds) / sizeof(*(*startAtom -> bonds)); i++)
        {
            if (isInStack(seenAtoms_stack))

            // check if bond is in stack
                // -> if true: do nothing, go next (gg go next)
                // -> else if new: recurse
        }

        // once end reach, go back a step (?) using magic
    }

    stack<Atom*>* getAtoms()
    {
        stack<Atom*>* atomPointer_stack;    // pointer to stack of pointers to unique atoms in molecule
        

        this -> recursiveSearch(this -> root, atomPointer_stack);
        
        return atomPointer_stack;
    }

    // Joins specified atom of this molecule to specified atom of other molecule
    void addMolecule(Molecule molecule, unsigned int sourceAtomID, unsigned int destinationAtomID)
    {

    }

    // Adds another bond between two atoms within the same molecule
    void addBond(unsigned int atomID_1, unsigned int atomID_2)
    {

    }

};
