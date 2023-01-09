#!/usr/bin/env python

import re
import json
import random
import castle
import createcharacter
import castleaction

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

def summary(game):
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

	if game.get("character").get("treasures")==[]:
		print("\nYou have no treasures")
	else:
		print("\nYou have the following treasures:")
		for treasure in game.get("character").get("treasures"):
			print(treasure)
	print("\n")



def enter_castle(character):

	castledata=castle.gen_castle()
	game={"character": character, "castle": castledata}


	regex=re.compile('[hnsewudmflogtq]|dr')
	yesnoregex=re.compile('[yn]')
	# Calling functions from dictionaries (better then if then else)
	choicedict={
		"h":  castleaction.castle_help,
		"n":  castleaction.go_north, 
		"s":  castleaction.go_south,
		"e":  castleaction.go_east,
		"w":  castleaction.go_west,
		"u":  castleaction.go_up,
		"d":  castleaction.go_down,
		"dr": castleaction.go_drink,
		"m":  castleaction.go_map,
		"f":  castleaction.go_flare,
		"l":  castleaction.go_lamp,
		"o":  castleaction.go_open,
		"g":  castleaction.go_gaze,
		"t":  castleaction.go_teleport
	}

	print("\nOk, " + game.get("character").get("race") + ", you are now entering the castle!\n")
	
	summary(game)
		
   
	exittheloop=False

	while exittheloop==False:

		game=castleaction.action_room(game)

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
					currentz=str(game.get("character").get("z"))
					found=False
					for contents in game.get("castle").get(currentx).get(currenty).get(currentz).get("contents"):
						if contents=="entrance":
							found=True
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
					if found==False:
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
			print("\n** Silly " + character.get("race") + ", that wasn't a valid command!\n")
			summary(game)
			game["character"]["moved"]=True  
			game=castleaction.action_room(game)

	
	return game.get("character")


def main():

	regex=re.compile("[yn]")
	print_intro()
	exittheloop=False
	while exittheloop==False:
		character=createcharacter.init_character()	  
	
		character=createcharacter.select_race(character)
		character=createcharacter.select_sex(character) 
		character=createcharacter.allocate_attributes(character)
		character=createcharacter.buy_equipment(character)
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

