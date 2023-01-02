import random


castlesize=8
entrancelocation="1/4/1"

warpran=8
sinkholeran=8
monsterran=4
chestran=6
vendorran=16
poolran=9

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
				castle[X][Y][level]={"contents": {}, "explored": False}
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
						
						
						chest=random.randint(0,chestran)
						if chest==0:
							castle[X][Y][level]["contents"]["chest"]={}
							gold=random.randint(0,2)
							if gold==0:
								castle[X][Y][level]["contents"]["chest"]["gold"]=random.randint(1,1000)
							book=random.randint(0,5)
							if book==0:
								castle[X][Y][level]["contents"]["chest"]["book"]=True
							orb=random.randint(0,3)
							if orb==0:
								castle[X][Y][level]["contents"]["chest"]["orb"]=True



	castle["1"]["4"]["1"]["contents"]={"entrance": 1}
	castle["1"]["4"]["1"]["explored"]=True
	return castle

