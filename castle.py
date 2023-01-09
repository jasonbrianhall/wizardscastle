import random
import createcharacter
import sys

castlesize=8
entrancelocation="1/4/1"

warpran=8
sinkholeran=8
monsterran=4
chestran=3
bookran=3
vendorran=16
poolran=9
orbran=8
vendorran=9
upran=7

def gen_castle():
	castle={}
	castle["size"]=castlesize
	monsters={
		"kobold": 
			{
				"name": "kobold",
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
				"name": "orc",
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
				"name": "wolf",
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
				"name": "goblin",
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
				"name": "ogre",
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
				"name": "troll",
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
				"name": "bear",
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
				"name": "minotaur",
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
				"name": "gargoyle",
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
				"name": "chimera",
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
				"name": "dragon",
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
				"name": "wizard's pet penguin",
				"strength": 18,
				"intelligence": 18,
				"dexterity": 3,
				"frequency": 1,
				"break": False,
				"spells": None

			},
		"giant spider":
			{
				"name": "giant spider",
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
	
	for x in range(1, castlesize+1):
		X=str(x)
		castle[X]={}
		for y in range(1, castlesize+1):
			Y=str(y)
			castle[X][Y]={}
			for z in range(1, castlesize+1):
				level=str(z)
				castle[X][Y][level]={"contents": {}, "explored": True}
				warp=random.randint(0,warpran)
				if warp==0:
					castle[X][Y][level]["contents"]["warp"]={}
				else:
					sinkhole=random.randint(0,sinkholeran)
					if sinkhole==0:
						castle[X][Y][level]["contents"]["sinkhole"]=True
				
				# You can't have monsters in warps or sink holes because that wouldn't make sense (except for the monster that guards the runestaff)
				if not (warp==0 or sinkhole==0):
				
					monster=random.randint(0,monsterran)
					if monster==0:
						currentmonster=random.choice(monsterlist)
						castle[X][Y][level]["contents"]["monster"]=monsters[currentmonster]
						try:
							modifier=castle[X][Y][level]["contents"]["monster"]["modifier"]
							strength=random.randint(-1*modifier, modifier)
							intelligence=random.randint(-1*modifier, modifier)
							dexterity=random.randint(-1*modifier, modifier)
							castle[X][Y][level]["contents"]["monster"]["strength"]=castle[X][Y][level]["contents"]["monster"]["strength"]+strength
							castle[X][Y][level]["contents"]["monster"]["intelligence"]=castle[X][Y][level]["contents"]["monster"]["strength"]+intelligence
							castle[X][Y][level]["contents"]["monster"]["dexterity"]=castle[X][Y][level]["contents"]["monster"]["strength"]+dexterity
						# No modifier defined; just don't modify the monster
						except:
							pass

					else:
						# vendors don't hang out with monsters
						vendor=random.randint(0,vendorran)
						if vendor==0:
							castle[X][Y][level]["contents"]["vendor"]={}

					upstairs=random.randint(0,upran)
					if upstairs==0:
						castle[X][Y][level]["contents"]["upstairs"]=True
					gold=random.randint(0,9)
					if gold==0:
						# No less then 5 gold is allowed if gold is found
						goldamount=random.randint(5,1000)
						castle[X][Y][level]["contents"]["gold"]=goldamount
					flares=random.randint(0,9)
					if gold==1:
						flaresamount=random.randint(1,20)
						castle[X][Y][level]["contents"]["flares"]=flaresamount
					chest=random.randint(0,chestran)
					if chest==0:
						castle[X][Y][level]["contents"]["chest"]={}
						explodes=random.randint(0,5)
						castle[X][Y][level]["contents"]["chest"]["opened"]=False
						if explodes==0:
							castle[X][Y][level]["contents"]["chest"]["explodes"]=True
						else:
							gold=random.randint(0,2)
							if gold==0:
								castle[X][Y][level]["contents"]["chest"]["gold"]=random.randint(1,1000)
							book=random.randint(0,5)
							if book==0:
								castle[X][Y][level]["contents"]["chest"]["book"]={}
								castle[X][Y][level]["contents"]["chest"]["book"]["title"]=genbook()
							orb=random.randint(0,5)
							if orb==0:
								castle[X][Y][level]["contents"]["chest"]["orb"]=True
							flare=random.randint(0,5)
							if flare==0:
								castle[X][Y][level]["contents"]["chest"]["flare"]=random.randint(1,20)

					book=random.randint(0,bookran)
					if book==0:
						castle[X][Y][level]["contents"]["book"]={}
						castle[X][Y][level]["contents"]["book"]["title"]=genbook()

					pool=random.randint(0,poolran)
					if pool==0:
						castle[X][Y][level]["contents"]["pool"]=True
							
					orb=random.randint(0,orbran)
					if orb==0:
						castle[X][Y][level]["contents"]["orb"]=True							


	castle["1"]["4"]["1"]["contents"]={"entrance": 1}
	castle["1"]["4"]["1"]["explored"]=True

	# calculate downstairs after entrance
	for x in range(1, castlesize+1):
		X=str(x)
		for y in range(1, castlesize+1):
			Y=str(y)
			for z in range(1, castlesize+1):
				level=str(z)
				if castle.get(X).get(Y).get(level).get("contents").get("upstairs"):
					temp=z+1
					if temp>castlesize:
						temp=1
					temp=str(temp)

					if castle.get(X).get(Y).get(temp).get("contents").get("sinkhole") or castle.get(X).get(Y).get(temp).get("contents").get("warp"):
						del castle[X][Y][level]["contents"]["upstairs"]
					else:
						castle[X][Y][temp]["contents"]["downstairs"]=True
	monsterlist.clear()

	# Put Runestaff in one of the rooms
	for x in range(1, castlesize+1):
		X=str(x)
		for y in range(1, castlesize+1):
			Y=str(y)
			for z in range(1, castlesize+1):
				Z=str(z)
				if castle.get(X).get(Y).get(Z).get("contents").get("monster"):
					monstertemp={"X": X, "Y": Y, "Z": Z}
					monsterlist.append(monstertemp)

	length=len(monsterlist)
	
	runestaffmonster=random.randint(0,length-1)
	X=monsterlist[runestaffmonster].get("X")
	Y=monsterlist[runestaffmonster].get("Y")
	Z=monsterlist[runestaffmonster].get("Z")
	
	castle[X][Y][Z]["contents"]["monster"]["runestaff"]=True
	
	warplist=[]

	# Put Orb of Zot in one of the warps
	for x in range(1, castlesize+1):
		X=str(x)
		for y in range(1, castlesize+1):
			Y=str(y)
			for z in range(1, castlesize+1):
				Z=str(z)
				if castle.get(X).get(Y).get(Z).get("contents").get("warp")=={}:
					warptemp={"X": X, "Y": Y, "Z": Z}
					warplist.append(warptemp)

	length=len(warplist)

	runestaffmonster=random.randint(0,length-1)
	X=warplist[runestaffmonster].get("X")
	Y=warplist[runestaffmonster].get("Y")
	Z=warplist[runestaffmonster].get("Z")
	
	castle[X][Y][Z]["contents"]["warp"]["orbofzot"]=True
	

	# Distribute Treasure	
	treasures=["Ruby red", "Pale pearl", "Green gem", "Opal eye", "Blue flame", "Norn stone", "Palantir", "Silmaril"]
	
	roomlist=[]
	# Put Orb of Zot in one of the warps
	for x in range(1, castlesize+1):
		X=str(x)
		for y in range(1, castlesize+1):
			Y=str(y)
			for z in range(1, castlesize+1):
				Z=str(z)
				roomvalid=True
				for content in castle.get(X).get(Y).get(Z).get("contents"):
					if content=="warp" or content=="sinkhole" or content=="entrance":
						roomvalid=False
				if roomvalid==True:
					special=False
					for content in castle.get(X).get(Y).get(Z).get("contents"):
						if content=="monster" or content=="vendor" or content=="chest" or content=="book":
							roomtemp={"X": X, "Y": Y, "Z": Z, "special": content}
							roomlist.append(roomtemp)
							special=True
					if special==False:
						roomtemp={"X": X, "Y": Y, "Z": Z, "special": None}
						roomlist.append(roomtemp)
			
			
	for treasure in range(0, len(treasures)-1):
		roomlength=len(roomlist)
		randomnumber=random.randint(0,roomlength-1)
		X=roomlist[randomnumber].get("X")
		Y=roomlist[randomnumber].get("Y")
		Z=roomlist[randomnumber].get("Z")
		if roomlist[randomnumber].get("special")==None:
			if castle.get(X).get(Y).get(Z).get("contents").get("treasure")==None:
				castle[X][Y][Z]["contents"]["treasure"]=[]
				castle[X][Y][Z]["contents"]["treasure"].append(treasures[treasure])
			else:
				castle[X][Y][Z]["contents"]["treasure"].append(treasures[treasure])

		else:
			content=roomlist[randomnumber].get("special")
			try:
				if castle.get(X).get(Y).get(Z).get("contents").get(content).get("treasure")==None:
					castle[X][Y][Z]["contents"][content]["treasure"]=[]
					castle[X][Y][Z]["contents"][content]["treasure"].append(treasures[treasure])
				else:
					castle[X][Y][Z]["contents"][content]["treasure"].append(treasures[treasure])	
			except:
				print(content + " not found")
				print(castle[X][Y][Z]["contents"][content])
				sys.exit(1)
	
		
	return castle

def genbook():
	sticks=random.randint(0,9)

	if sticks==0:
		return "sticks"
	else:
		content=random.randint(0,9)
		if content==0:
			return"strength"
		elif content==1:
			return "dexterity"
		elif content==2:
			return "intelligence"
		elif content==3:
			return "blind"
		elif content==4:
			return "It'S another volume of Zot'S Poetry! - Yech!!"
		elif content==5:
			return "It'S in a foreign language; you can't read it!"
		elif content==6:
			return "It's an old dictionary!"
		elif content==7:
			famousbooks={
				"0": "Lord of the Rings",
				"1": "The Hobbit",
				"2": "Lord of the Flies",
				"3": "Moby Dick",
				"4": "Pinocchio",
				"5": "Grimm Tales",
				"6": "the rules to Magic the Gathering",
				"7": "the rules to Dungeons and Dragons",
				"8": "Dune",
				"9": "King Arthur's biography",
				"10": "the LIttle Mermaid"
				}
			test=random.randint(0,10)
			return "It'S a copy of " + famousbooks[str(test)]+ "!"

		elif content>=8:
			temp=createcharacter.races
			list=[]
			for x in temp:
				list.append(temp.get(x).get("name"))
			return "It'S an old copy of play " + random.choice(list)


