import re

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
					choice=input()
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
		print("You have " + str(character.get("gold")) + " gold pieces.\n")
		 
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
		exitloop=False
		while exitloop==False:
			print("Flares cost 1 GP each.  How many do you want? ", end="")
			choice=input()
			if re.match(regex, choice):
				if int(choice)>character.get("gold"):
					print("\n** You can only afford " + str(character.get("gold")))
				else:
					character["flares"]=character.get("flares")+int(choice)
					character["gold"]=character.get("gold")-int(choice)
					exitloop=True
			else:
				print("\n** If you don't want any, just type 0 (zero)\n")


	return character


