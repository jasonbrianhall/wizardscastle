import random
import createcharacter

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
					else:
						# vendors don't hang out with monsters
						vendor=random.randint(0,vendorran)
						if vendor==0:
							castle[X][Y][level]["contents"]["vendor"]={}

					upstairs=random.randint(0,upran)
					if upstairs==0:
						castle[X][Y][level]["contents"]["upstairs"]=True

					chest=random.randint(0,chestran)
					if chest==0:
						castle[X][Y][level]["contents"]["chest"]={}
						explodes=random.randint(0,5)
						if explodes==0:
							castle[X][Y][level]["contents"]["chest"]["explodes"]=True
						else:
							gold=random.randint(0,2)
							if gold==0:
								castle[X][Y][level]["contents"]["chest"]["gold"]=random.randint(1,1000)
							book=random.randint(0,5)
							if book==0:
								castle[X][Y][level]["contents"]["chest"]["book"]=True
							orb=random.randint(0,3)
							if orb==0:
								castle[X][Y][level]["contents"]["chest"]["orb"]=True
							flare=random.randint(0,3)
							if flare==0:
								castle[X][Y][level]["contents"]["chest"]["flare"]=random.randint(1,20)

					book=random.randint(0,bookran)
					if book==0:
						castle[X][Y][level]["contents"]["book"]={}
						sticks=random.randint(0,5)
						if sticks==0:
							castle[X][Y][level]["contents"]["book"]["sticks"]=True
						else:
							content=random.randint(0,9)
							if content==0:
								castle[X][Y][level]["contents"]["book"]["content"]="strength"
							elif content==1:
								castle[X][Y][level]["contents"]["book"]["content"]="dexterity"
							elif content==2:
								castle[X][Y][level]["contents"]["book"]["content"]="intelligence"
							elif content==3:
								castle[X][Y][level]["contents"]["book"]["content"]="blind"
							elif content==4:
								castle[X][Y][level]["contents"]["book"]["content"]="It'S another volume of Zot'S Poetry! - Yech!!"
							elif content==5:
								castle[X][Y][level]["contents"]["book"]["content"]="It'S in a foreign language; you can't read it!"
							elif content==6:
								castle[X][Y][level]["contents"]["book"]["content"]="It's an old dictionary!"
							elif content==7:
								castle[X][Y][level]["contents"]["book"]["content"]="It'S too boring to read!"
							elif content>=8:
								temp=createcharacter.races
								list=[]
								for x in temp:
									list.append(temp.get(x).get("name"))
								castle[X][Y][level]["contents"]["book"]["content"]="It'S an old copy of play " + random.choice(list)

					pool=random.randint(0,poolran)
					if pool==0:
						castle[X][Y][level]["contents"]["pool"]=True
							
					orb=random.randint(0,orbran)
					if orb==0:
						castle[X][Y][level]["contents"]["orb"]=True							


	castle["1"]["4"]["1"]["contents"]={"entrance": 1}
	castle["1"]["4"]["1"]["explored"]=True

	# calculate downstairs after entrance

	
	for x in range(1,castlesize+1):
		print(x)

	for x in range(1, castlesize+1):
		X=str(x)
		for y in range(1, castlesize+1):
			Y=str(y)
			for z in range(1, castlesize+1):
				level=str(z)
				print(X, Y, level, castle.get(X).get(Y).get(level).get("contents").get("upstairs"))
				if castle.get(X).get(Y).get(level).get("contents").get("upstairs"):
					temp=z+1
					if temp>castlesize:
						temp=1
					temp=str(temp)

					if castle.get(X).get(Y).get(temp).get("contents").get("sinkhole") or castle.get(X).get(Y).get(temp).get("contents").get("warp"):
						del castle[X][Y][level]["contents"]["upstairs"]
						print("Deleting Upstairs " + X + "/"+Y+"/"+temp)
					else:
						castle[X][Y][temp]["contents"]["downstairs"]=True
						print("Adding downstairs " + X + "/"+Y+"/"+temp)
					

	return castle

