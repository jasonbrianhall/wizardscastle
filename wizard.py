#!/usr/bin/env
import re
import json

castlesize=8
entrancelocation="1/4/1"

def gen_castle():
    castle={}

    for x in range(1, castlesize):
        X=str(x)
        for y in range(1, castlesize):
            Y=str(y)
            for z in range(1, castlesize):
                Z=str(z)
                castle[X+"/"+Y+"/"+Z]={"contents": "empty", "explored": False}
               
               
    castle[entrancelocation]["contents"]="entrance"
    
        

def init_character():
    
    character={}
    character["race"]             = ""
    character["strength"]         = 0
    character["dexterity"]        = 0
    character["intelligence"]     = 0
    character["allocate"]         = 0
    character["gold"]             = 0
    character["bookstucktohand"]  = False
    character["sex"]              = ""
    character["treasures"]        = []
    character["friendlyvendors"]  = True
    character["weapons"]          = "Nothing"
    character["armor"]            = "Nothing"
    character["lamp"]             = True
    character["flares"]           = 0
    character["x"]                = 1
    character["y"]                = 4
    character["z"]                = 1
    
    return character

def buy_equipment(character):

    armory={
        "armor":   {
            "regex": "[pcln]",
            "selection": {"p": "Plate", "c": "Chainmail", "l": "Leather", "n": "Nothing"}, 
            "equipment": {"Plate":  30, "Chainmail": 20, "Leather": 10, "Nothing": 0},
            },
        "weapons": {
            "regex": "[smdn]",
            "selection": {"s": "Sword", "m": "Mace", "d": "Dagger", "n": "Nothing"}, 
            "equipment": {"Sword":  30, "Mace":      20, "Dagger":  10, "Nothing": 0}
            }
        }

    print("Ok, " + character.get("race") + ", you have " + str(character.get("gold")) + " gold pieces (GP\'s).\n")

    for arms in armory:
        regex=re.compile(armory.get(arms).get("regex"))

        if character.get("gold")>0:
            print("These are the types of " + arms + " you can buy.")
            counter=0
            for arm in armory.get(arms).get("equipment"):
                if counter==0:
                    print(arm + "<" + str(armory.get(arms).get("equipment").get(arm)) + ">", end="")
                    counter=1
                else:
                    print(" " + arm + "<" + str(armory.get(arms).get("equipment").get(arm)) + ">", end="")
            print("\n\nYour choice? ", end="")
            data=""
            while data=="":
                try:
                    choice=input()[0].lower()
                except:
                    choice=""
                if re.match(regex, choice):
                    data="data"
                    character[arms]=armory.get(arms).get("selection").get(choice)
                    character["gold"]=character.get("gold")-armory.get(arms).get("equipment").get(character.get(arms))
                else:
                    print("\n** Is your IQ really " + str(character.get("intelligence")) + " ?\n")
                    print("These are the types of " + arms + " you can buy.")
                    counter=0
                    for arm in armory.get(arms).get("equipment"):
                        if counter==0:
                            print(arm + "<" + str(armory.get(arms).get("equipment").get(arm)) + ">", end="")
                            counter=1
                        else:
                            print(" " + arm + "<" + str(armory.get(arms).get("equipment").get(arm)) + ">", end="")
    if character.get("gold")>=20:
        regex=re.compile("[yn]")
        data=False
        while data==False:
            print("Do you want to buy a lamp for 20 GP's? ", end="")
            try:
                choice=input()[0].lower()
            except:
                choice=""
                pass
            if re.match(regex, choice):
                character["lamp"]=True
                character["gold"]=character.get("gold")-20
                data=True
            else:
                print("\n** Please answer Yes or No\n")
 
    if character.get("gold")>0:
        regex=re.compile("[0-9]+")
        print("Ok, " + character.get("race") + ", you have " + str(character.get("gold")) + " gold pieces left.\n")
        data=False
        while data==False:
            print("Flares cost 1 GP each.  How many do you want? ", end="")
            choice=input()
            if re.match(regex, choice):
                if int(choice)>character.get("gold"):
                    print("\n** You can only afford " + str(character.get("gold")))
                else:
                    character["flares"]=character["flares"]+int(choice)
                    data=True
            else:
                print("\n** If you don't want any, just type 0 (zero)\n")


    return character


def allocate_attributes(character):

    regex=re.compile("[0-9]+")

    print("Ok,", character.get("race"), "you have the following attributes :")
    print("Strength = ", character.get("strength"), "Intelligence = ", character.get("intelligence"), "Dexterity = ", character.get("dexterity"))
    print("and ", character.get("allocate"), "other points to allocate as you wish.")

    for x in ["strength", "intelligence", "dexterity"]:
        if character.get("allocate")>0:
            print("\nHow many points do you wish to add to your " + x + "? ", end="")
            data=False
            while data==False:
                try:
                    choice=input()[0]
                except:
                    choice=""
                if re.match(regex, choice) and int(choice)>=0 and int(choice)<=character.get("allocate"):
                    value=int(choice)
                    character[x]=character[x]+value
                    character["allocate"]=character["allocate"]-value
                    data=True
                else:
                    print("\n** How many points do you wish to add to your " + x + "? ", end="")
    return character


def select_race(character):
    
    regex=re.compile("[edmh]")
    datamapper={
        "e": {"strength":  6, "intelligence": 8, "dexterity": 10, "name": "Elf",    "allocate": 8, "gold": 60},
        "d": {"strength": 10, "intelligence": 8, "dexterity":  6, "name": "Dwarf",  "allocate": 8, "gold": 60},
        "m": {"strength":  8, "intelligence": 8, "dexterity":  8, "name": "Human",    "allocate": 8, "gold": 60},
        "h": {"strength":  4, "intelligence": 8, "dexterity": 12, "name": "Hobbit", "allocate": 4, "gold": 60}
        }

    print("All right, bold one.")

    choice=""
    race=""

    while(race.lower()==""):
        print("You may be an Elf, Dwarf, Man, or Hobbit")

        try:
            print("\nYour Choice? ", end="")
            choice=input()[0].lower()
        except:
            pass
        if  re.match(regex, choice):
            race=choice
        else:
            print("** That was incorrect. Please type E, D, M, or H.")

    character["race"]         = datamapper.get(race).get("name")
    character["strength"]     = datamapper.get(race).get("strength")
    character["dexterity"]    = datamapper.get(race).get("dexterity")
    character["intelligence"] = datamapper.get(race).get("intelligence")
    character["allocate"]     = datamapper.get(race).get("allocate")
    character["gold"]         = datamapper.get(race).get("gold")

    return character


def select_sex(character):
    
    sex=""

    datamapper={"m": "Male", "f": "Female"}
    regex=re.compile("[mf]")

    choice=""
    while(sex.lower()==""):
        try:
            print("\nWhich sex do you prefer? ", end="")
            choice=input()[0].lower()
        except:
            pass
        if re.match(regex, choice):
            sex=datamapper.get(choice)
        else:
            print("** Cute", character["race"], "real cute.  Try M or F.")

    character["sex"]=sex
    return character

def print_intro():
    # Print Intro

    buffer=""
    buffer=buffer+"*"*64

    buffer=buffer+"\n\n"
    buffer=buffer+" "*16
    buffer=buffer+"* * * THE WIZARD'S CASTLE * * *\n\n"

    buffer=buffer+"*"*64

    buffer=buffer+"\n"*2
    buffer=buffer+"""Many cycles ago, in the kindom of N'Dic, the gnomic
wizard zot forged his great *Orb of Power*. He soon
vanished, leaving behind his vast subterranean castle
filled with esurrient monsters, fabulous treasures, and
the incredible *Orb of Zot*. From that time hence, many
a bold youth has ventured into the wizard's castle. as
of now, *NONE* has ever emerged victoriously! Beware!!\n"""

    print(buffer)

def castle_help():

    buffer=""""*** WIZARD'S CASTLE COMMAND AND INFORMATION SUMMARY ***

The following commands are available :

H/elp     N/orth    S/outh    E/ast     W/est     U/p
D/own     DR/ink    M/ap      F/lare    L/amp     O/pen
G/aze     T/eleport Q/uit

the contents of rooms are as follows :"

. = empty room      b = book            c = chest
d = stairs down     e = entrance/exit   f = flares
g = gold pieces     m = monster         o = crystal orb
p = magic pool      s = sinkhole        t = treasure
u = stairs up       v = vendor          w = warp/orb

The benefits of having treasures are :

Ruby red - avoid lethargy     Pale pearl - avoid leech
Green gem - avoid forgetting  Opal eye - cures blindness
Blue flame - dissolves books  Norn stone - no benefit
Palantir - no benefit         Silmaril - no benefit

Press return when ready to resume"""

    print(buffer)

    input()

def enter_castle(character, castle):

    print("Ok, " + character.get("race") + ", you are now entering the castle!\n")
    
    print("You are at ( " + str(character.get("x")) + ", " + str(character.get("y")) + " ) Level " + str(character.get("z")) + "\n")
    print("Strength = " + str(character.get("strength")), end="")
    print("  Intelligence = " + str(character.get("intelligence")), end="")
    print("  Dexterity = " + str(character.get("dexterity")))
    print("Treasures = " + str(len(character.get("treasures"))), end="")
    print(" Flares = " + str(character.get("flares")), end="")
    print(" Gold pieces = " + str(character.get("gold")))
    if not character.get("weapons")=="nothing":
        print("Weapon = " + character.get("weapons"), end="")
    else:
        print("Weapon = No weapon", end="")
    if not character.get("armor")=="nothing":
        print("  Armor = " + character.get("armor"))
    else:
        print("  Armor = No armor", end="")
   
    castle_help()
    
    
    return character


def main():

    character=init_character()    
    castle=gen_castle()
    
    print_intro()
 
    character=select_race(character)
    character=select_sex(character) 
    character=allocate_attributes(character)
    character=buy_equipment(character)
    character=enter_castle(character, castle)
    
    print(json.dumps(character, indent=4))

main()

