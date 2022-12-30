#!/usr/bin/env
import re

def allocate_attributes(character):

	regex=re.compile("[0-9]+")

	print("Ok,", character.get("race"), "you have the following attributes :")
	print("Strength = ", character.get("strength"), "Intelligence = ", character.get("intelligence"), "Dexterity = ", character.get("dexterity"))
	print("and ", character.get("allocate"), "other points to allocate as you wish.")

	for x in ["strength", "intelligence", "dexterity"]:
		if character.get("allocate")>0:
			print("\nHow many points do you wish to add to your " + x + "?")
			data=""
			while data=="":
				try:
					choice=input()[0]
				except:
					choice=""
				if re.match(regex, choice) and int(choice)>=0 and int(choice)<=character.get("allocate"):
					value=int(choice)
					character[x]=character[x]+value
					character["allocate"]=character["allocate"]-value
					data="not empty"
				else:
					print("\n** How many points do you wish to add to your " + x + "?")
			


def select_race(character):
	

	regex=re.compile("[edmh]")
	datamapper={
		"e": {"strength":  6, "intelligence": 8, "dexterity": 10, "name": "Elf",	"allocate": 8},
		"d": {"strength": 10, "intelligence": 8, "dexterity":  6, "name": "Dwarf",	"allocate": 8},
		"m": {"strength":  8, "intelligence": 8, "dexterity":  8, "name": "Man",	"allocate": 8},
		"h": {"strength":  4, "intelligence": 8, "dexterity": 12, "name": "Hobbit", "allocate": 4}
		}

	print("All right, bold one.")

	choice=""
	race=""

	while(race.lower()==""):
		print("You may be an Elf, Dwarf, Man, or Hobbit")

		try:
			print("\nYour Choice? ")
			choice=input()[0].lower()
		except:
			pass
		if	re.match(regex, choice):
			race=choice
		else:
			print("** That was incorrect. Please type E, D, M, or H.")

	character["race"]		  =datamapper.get(race).get("name")
	character["strength"]	  =datamapper.get(race).get("strength")
	character["dexterity"]	  =datamapper.get(race).get("dexterity")
	character["intelligence"] =datamapper.get(race).get("intelligence")
	character["allocate"]	  =datamapper.get(race).get("allocate")

	return character


def select_sex(character):
	
	sex=""

	datamapper={"m": "Male", "f": "Female"}
	regex=re.compile("[mf]")

	choice=""
	while(sex.lower()==""):
		try:
			print("\nWhich sex do you prefer? ")
			choice=input()[0].lower()
		except:
			pass
		if re.match(regex, choice):
			sex=datamapper.get(choice)
		else:
			print("** Cute", character["race"], "real cute.	 Try M or F.")

	character["sex"]=sex
	return character

def print_intro():
		# Print Intro

	buffer=""
	buffer=buffer+"*"*64

	buffer=buffer+("\n\n				* * * THE WIZARD'S CASTLE * * *\n\n")

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


def main():

	character={}	
	
	print_intro()
	character=select_race(character)
	character=select_sex(character)	
	character=allocate_attributes(character)

main()

