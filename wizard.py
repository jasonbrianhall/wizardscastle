#!/usr/bin/env python

import re
import json
import random

castlesize=8
entrancelocation="1/4/1"

warpran=8
sinkholeran=8
monsterran=5
chestran=6
vendorran=16
poolran=9

def gen_castle():
	castle={}
	
	monsters={
		"kobold": 
			{
				"modifier": 3,
				"strength": 5,
				"intelligence": 4,
				"dexterity": 7,
				"frequency": 8,
				"break": False,
				"spells": None
				
				
			},
		"orc":
			{
				"modifier": 3,
				"strength": 16,
				"intelligence": 2,
				"dexterity": 4,
				"frequency": 6,
				"break": False,
				"spells": None
			},
		"wolf":
			{
				"modifier": 3,
				"strength": 10,
				"intelligence": 14,
				"dexterity": 14,
				"frequency": 8,
				"break": False,
				"spells": None
			},
		"goblin":
			{
				"modifier": 3,
				"strength": 5,
				"intelligence": 3,
				"dexterity": 3,
				"frequency": 8,
				"break": False,
				"spells": None
			},
		"ogre":
			{
				"modifier": 3,
				"strength": 20,
				"intelligence": 3,
				"dexterity": 4,
				"frequency": 6,
				"break": False,
				"spells": None
			},
		"troll":
			{
				"modifier": 3,
				"strength": 7,
				"intelligence": 7,
				"dexterity": 9,
				"frequency": 8,
				"break": False,
				"spells": None

			},
		"bear":
			{
				"modifier": 3,
				"strength": 15,
				"intelligence": 4,
				"dexterity": 5,
				"frequency": 6,
				"break": False,
				"spells": None

			},
		"minotaur":
			{
				"modifier": 3,
				"strength": 11,
				"intelligence": 15,
				"dexterity": 13,
				 "frequency": 4,
				"break": False,
				"spells": None

			},
		"gargoyle":
			{
				"modifier": 3,
				"strength": 13,
				"intelligence": 4,
				"dexterity": 6,
				"frequency": 8,
				# Stone breaks armor
				"break": True,
				"spells": None

			},
		"chimera":
			{
				"modifier": 3,
				"strength": 32,
				"intelligence": 14,
				"dexterity": 18,
				"frequency": 8,
				"break": False,
				"spells": None

			},
		"dragon":
			{
				"modifier": 10,
				"strength": 40,
				"intelligence": 18,
				"dexterity": 17,
				"frequency": 4,
				# Dragon scales break armor
				"break": True,
				"spells": None,

			},
		"wizard's pet penguin":
			{
				"strength": 18,
				"intelligence": 18,
				"dexterity": 3,
				"frequency": 1,
				"break": False,
				"spells": None

			},
		"giant spider":
			{
				"strength": 18,
				"intelligence": 18,
				"dexterity": 13,
				"frequency": 2,
				"break": False,
				"spells": None
			}
	}
	
	monsterlist=[]
	for x in monsters:
		for frequency in range(1, monsters.get(x).get("frequency")):
			monsterlist.append(x)
	

	for z in range(1, castlesize):
		level=str(z)
		castle[level]={}
		for y in range(1, castlesize):
			Y=str(y)
			castle[level][Y]={}
			for x in range(1, castlesize):
				X=str(x)
				castle[level][Y][X]={"contents": {}, "explored": False}
				warp=random.randint(0,warpran)
				if warp==0:
					castle[level][Y][X]["contents"]["warp"]={}
				else:
					sinkhole=random.randint(0,sinkholeran)
					if sinkhole==0:
						castle[level][Y][X]["contents"]["sinkhole"]=True
				
				# You can't have monsters in warps or sink holes because that wouldn't make sense (except for the monster that guards the runestaff)
				if not (warp==0 or sinkhole==0):
				
					monster=random.randint(0,monsterran)
					if monster==0:
						currentmonster=random.choice(monsterlist)
						castle[level][Y][X]["contents"]["monster"]=monsters[currentmonster]
						
						
						chest=random.randint(0,chestran)
						if chest==0:
							castle[level][Y][X]["contents"]["chest"]={}
							gold=random.randint(0,2)
							if gold==0:
								castle[level][Y][X]["contents"]["chest"]["gold"]=random.randint(1,1000)
							book=random.randint(0,5)
							if book==0:
								castle[level][Y][X]["contents"]["chest"]["book"]=True
							orb=random.randint(0,3)
							if orb==0:
								castle[level][Y][X]["contents"]["chest"]["orb"]=True



	castle["1"]["4"]["1"]["contents"]={"entrance": 1}
	return castle

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
	character["weapons"]          = {}
	character["armor"]            = {}
	character["lamp"]             = True
	character["flares"]           = 0
	character["x"]                = 1
	character["y"]                = 4
	character["z"]                = 1
	character["turns"]            = 0
	character["moved"]            = False
	character["orbofzot"]         = False
	character["runestaff"]        = False
	return character

def buy_equipment(character):

	armory={
		"armor":   {
			"regex": "[pcln]",
			"selection": {"p": "Plate", "c": "Chainmail", "l": "Leather", "n": "Nothing"},
			"equipment": {
				"Plate": {
					"cost": 30,
					"effect": 6
				},
				"Chainmail": {
					"cost": 20,
					"effect": 4
				},	
				"Leather": {
					"cost": 10,
					"effect": 2
				},
				"Nothing": {
					"cost": 0,
					"effect": 0
				}

			}
		},
		"weapons": {
			"regex": "[smdn]",
			"selection": {"s": "Sword", "m": "Mace", "d": "Dagger", "n": "Nothing"}, 
			"equipment": {
				"Sword": {
					"cost": 30,
					"effect": 6
				},
				"Mace": {
					"cost": 20,
					"effect": 4
				},
				"Dagger": {
					"cost": 10,
					"effect": 2
				},
				"Nothing": {
					"cost": 0,
					"effect": 0
				}
			}
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
					print(arm + "<" + str(armory.get(arms).get("equipment").get(arm).get("cost")) + ">", end="")
					counter=1
				else:
					print(" " + arm + "<" + str(armory.get(arms).get("equipment").get(arm).get("cost")) + ">", end="")
			print("\n\nYour choice? ", end="")
			exitloop=False
			while exitloop==False:
				try:
					choice=input()[0].lower()
				except:
					choice=""
				if re.match(regex, choice):
					exitloop=True
					character[arms]["name"]=armory.get(arms).get("selection").get(choice)
					character[arms]["effect"]=armory.get(arms).get("equipment").get(armory.get(arms).get("selection").get(choice)).get("effect")
					
					character["gold"]=character.get("gold")-armory.get(arms).get("equipment").get(armory.get(arms).get("selection").get(choice)).get("cost")
				else:
					print("\n** Is your IQ really " + str(character.get("intelligence")) + " ?\n")
					print("These are the types of " + arms + " you can buy.")
					counter=0
					for arm in armory.get(arms).get("equipment"):
						if counter==0:
							print(arm + "<" + str(armory.get(arms).get("equipment").get(arm).get("cost")) + ">", end="")
							counter=1
						else:
							print(" " + arm + "<" + str(armory.get(arms).get("equipment").get(arm).get("cost")) + ">", end="")
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
		"m": {"strength":  8, "intelligence": 8, "dexterity":  8, "name": "Human",  "allocate": 8, "gold": 60},
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
		if	re.match(regex, choice):
			race=choice
		else:
			print("** That was incorrect. Please type E, D, M, or H.")

	character["race"]         = datamapper.get(race).get("name")
	character["strength"]     = datamapper.get(race).get("strength")
	character["dexterity"]	  = datamapper.get(race).get("dexterity")
	character["intelligence"] = datamapper.get(race).get("intelligence")
	character["allocate"]	  = datamapper.get(race).get("allocate")
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
			print("** Cute", character["race"] +" real cute.  Try M or F.")

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
wizard Zot forged his great *Orb of Power*. He soon
vanished, leaving behind his vast subterranean castle
filled with esurrient monsters, fabulous treasures, and
the incredible *Orb of Zot*. From that time hence, many
a bold youth has ventured into the wizard's castle. as
of now, *NONE* has ever emerged victoriously! Beware!!\n"""

	print(buffer)

def castle_help(game):

	print(""""*** WIZARD'S CASTLE COMMAND AND INFORMATION SUMMARY ***

The following commands are available :

H/elp	  N/orth	S/outh	  E/ast		W/est	  U/p
D/own	  DR/ink	M/ap	  F/lare	L/amp	  O/pen
G/aze	  T/eleport Q/uit

the contents of rooms are as follows :"

. = empty room		b = book			c = chest
d = stairs down		e = entrance/exit	f = flares
g = gold pieces		m = monster			o = crystal orb
p = magic pool		s = sinkhole		t = treasure
u = stairs up		v = vendor			w = warp/orb

The benefits of having treasures are :

Ruby red - avoid lethargy	  Pale pearl - avoid leech
Green gem - avoid forgetting  Opal eye - cures blindness
Blue flame - dissolves books  Norn stone - no benefit
Palantir - no benefit		  Silmaril - no benefit

Press return when ready to resume""")

	input()
	
	return game

def go_south(game):
	game["character"]["y"]=game.get("character").get("y")-1
	if game.get("character").get("y")<1:
		game["character"]["y"]=8
	game["character"]["moved"]=True
	return game

def go_north(game):
	game["character"]["y"]=game.get("character").get("y")+1
	if game.get("character").get("y")>8:
		game["character"]["y"]=1
	game["character"]["moved"]=True
	return game

def go_east(game):
	game["character"]["x"]=game.get("character").get("x")+1
	if game.get("character").get("x")>8:
		game["character"]["x"]=1
	game["character"]["moved"]=True
	return game

def go_west(game):
	game["character"]["x"]=game.get("character").get("x")-1
	if game.get("character").get("x")<1:
		game["character"]["y"]=8
	game["character"]["moved"]=True
	return game

def go_up(game):
	return game

def go_down(game):
	return game
	
def go_drink(game):
	return game
	
def go_map(game):
	return game

def go_flare(game):
	return game

def go_lamp(game):
	return game
	
def go_open(game):
	return game

def go_gaze(game):
	return game

def go_teleport(game):
	return game

def ambience():
	print("\nYou ", end="")

	data={
		"0": {
			"init": "stepped on a",
			"random": [
				" frog!",
				" toad!",
				" fairie!",
				" brownie!",
				" a chicken egg!"
			],
			"post": [
				" You wipe it off your boots.",
				"",
				"",
				" You are reminded to buy new boots if you live."
				]
		},
		"1": {
			"init": "hear",
			"random": [
				" a scream!",
				" footsteps!",
				" a wumpus!",
				" thunder!",
				" a scream!	 You think someone is being tortured!"
				],
			"post": [
				" Your heart starts racing.",
				"",
				"",
				"",
				"",
				" You want to go home."
				]
			},
		"2": {
			"init": "sneezed!",
			"random": [
				" You think a monster heard you!",
				" Now you have to pee!",
				"",
				"",
				""
				],
			"post": [
				" You think about trying to make less noise.",
				"",
				"",
				"",
				"",
				" Now you farted."
				]
			},
		"3": {
			"init": "see a bat fly by!",
			"random": [
				" Pray it's not a vampire bat.",
				""
				],
			"post": [
				" It flies a little too close for comfort.",
				"",
				"",
				""
				]
			},
		"4": {
			"init": "smell",
			"random": [
				" a kobold",
				" an orc",
				" a wolf",
				" a goblin",
				" an ogre",
				" a troll",
				" a bear",
				" a minotaur",
				" a gargoyle",
				" a chimera",
				" a dragon",
				],
			"post": [
				" frying!!!",
				" roasting!!!",
				" cooking!!!",
				"\'s entrails!",
				"\'s stool!",
				"\'s breathe nearby!"
				]
			},
		"5": {
			"init": "feel like you are being watched!",
			"random": [
				" You see glowing eyes!",
				" You think you hear someone talking."
				],
			"post": [
				" You realize it's only in your head.",
				" You swallow your fear."
				"",
				"",
				"",
				""
				]
			},
		"6": {
			"init": "hear faint rustling noises!",
			"random": [
				" They are getting closer!",
				""
				],
			"post": [
				" Now they are getting further away.",
				"",
				"",
				""
				]
			},
		"7": {
			"init": "feel like you are being watched!",
			"random": [
				" You see glowing eyes!",
				" You think you hear someone talking.",
				"",
				"",
				"",
				"",
				""
				],
			"post": [
				" You think you are going insane",
				""
				""
				""
				]
			},
		}
	



	randata=random.randint(0,10)
	if randata>7:
		randata=4

	ranstring=str(randata)
	mylist=data.get(ranstring).get("random")
	postlist=data.get(ranstring).get("post")
	
	print(data.get(ranstring).get("init") + random.choice(mylist) + random.choice(postlist))

def action_room(game):
	if game.get("character").get("moved")==True:
		print("Your current position is: " + str(game.get("character").get("x"))+"/"+str(game.get("character").get("y"))+"/"+str(game.get("character").get("z")))
		game["character"]["moved"]=False
		ambience()
			
	return game



def enter_castle(character):

	castle=gen_castle()
	game={"character": character, "castle": castle}


	regex=re.compile('[hnsewudmflogtq]|dr')
	yesnoregex=re.compile('[yn]')
	# Calling functions from dictionaries (better then if then else)
	choicedict={
		"h":  castle_help,
		"n":  go_north, 
		"s":  go_south,
		"e":  go_east,
		"w":  go_west,
		"u":  go_up,
		"d":  go_down,
		"dr": go_drink,
		"m":  go_map,
		"f":  go_flare,
		"l":  go_lamp,
		"o":  go_open,
		"g":  go_gaze,
		"t":  go_teleport
	}

	print("\nOk, " + game.get("character").get("race") + ", you are now entering the castle!\n")
	
	print("You are at ( " + str(game.get("character").get("x")) + ", " + str(game.get("character").get("y")) + " ) Level " + str(game.get("character").get("z")) + "\n")
	print("Strength = " + str(game.get("character").get("strength")), end="")
	print("	 Intelligence = " + str(game.get("character").get("intelligence")), end="")
	print("	 Dexterity = " + str(game.get("character").get("dexterity")))
	print("Treasures = " + str(len(game.get("character").get("treasures"))), end="")
	print(" Flares = " + str(game.get("character").get("flares")), end="")
	print(" Gold pieces = " + str(game.get("character").get("gold")))
	if not game.get("character").get("weapons").get("name")=="nothing":
		print("Weapon = " + game.get("character").get("weapons").get("name"), end="")
	else:
		print("Weapon = No weapon", end="")
	if not game.get("character").get("armor")=="nothing":
		print("	 Armor = " + game.get("character").get("armor").get("name"))
	else:
		print("	 Armor = No armor")
   
	exittheloop=False

	while exittheloop==False:

		game=action_room(game)

		print("\nEnter your command : ", end="")
		try:
			choice=input().lower()
			if len(choice)>=2: 
				choice=choice[:2]
				if not choice=="dr":
					choice=choice[:1]
			else:
				choice=choice[:1]
		except:
			choice=""
		game["character"]["turns"]=game["character"]["turns"]+1
		if re.match(regex, choice):
			if choice=="q" or choice=="n":
				if choice=="n":
					quitloop=False
								
					currentx=str(game.get("character").get("x"))
					currenty=str(game.get("character").get("y"))
					currentz=str(game.get("character").get("x"))
					for contents in game.get("castle").get(currentx).get(currenty).get(currentz).get("contents"):
						if contents=="entrance":
							quitloop=False
							while quitloop==False:
								print("This is the exit; are you sure you want to leave? ", end="")
								newchoice=input()[:1].lower()
								if re.match(yesnoregex, newchoice):
									if newchoice=="y":
										if game.get("character").get("orbofzot")==False:
											print("\nYou left the castle without the Orb of Zot.")
										exittheloop=True
										quitloop=True
									else:
										quitloop=True
								else:
									print("\n** Invalid choice stupid " + game.get("character").get("race") + "; try y or n.\n")
						else:
							game=choicedict.get(choice)(game)									
				else:
					quitloop=False
					while quitloop==False:
						print("Do you really want to quit now? ", end="")
						try:
							newchoice=input()[:1].lower()
						except:
							newchoice=""
						if re.match(yesnoregex, newchoice) and newchoice=="y":								
							exittheloop=True 
							quitloop=True
						elif re.match(yesnoregex, newchoice) and newchoice=="n":
							quitloop=True
						else:
							print("** Please enter yes or no.\n")
				
				if exittheloop==True:
					if game.get("character").get("orbofzot")==True and choice=="n":
						print("An incredibly glorious victory!!\n")
					else:
						print("\nA less than awe-inspiring defeat.\n")

					print("When you left the castle, you had :\n\nYour miserable life!")
					print(game.get("character").get("weapons").get("name") + " and " + game.get("character").get("armor").get("name"))
					print("\nYou also had " + str(game.get("character").get("flares")) + " flares and " + str(game.get("character").get("gold")) + " gold pieces", end="")
					if game.get("character").get("runestaff")==True:
						print(" and the runestaff", end="")
			
					print("\n\nAnd it took you " + str(game.get("character").get("turns")) + " turns!\n")

				
			else:
				game=choicedict.get(choice)(game)
		else:
			print("\n** Silly " + character.get("race") + ", that wasn't a valid command!")
			
		
	
	
	return game.get("character")


def main():

	regex=re.compile("[yn]")
	print_intro()
	exittheloop=False
	while exittheloop==False:
		character=init_character()	  
	
		character=select_race(character)
		character=select_sex(character) 
		character=allocate_attributes(character)
		character=buy_equipment(character)
		character=enter_castle(character)

		exitloop2=False
		while exitloop2==False:
			print("\nAre you foolish enough to want to play again? ", end="")
			try:
				choices=input()[:1]
			except:
				choices=""
			if re.match(regex, choices):

				exitloop2=True

				if choices=="n":
					print("\nMaybe dumb " + character.get("race") + " is not so dumb after all!")
					exittheloop=True
				else:
					print("\n")
			else:
				print("** Please answer yes or no")
		
	print(json.dumps(character, indent=4))

main()

