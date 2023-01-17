import random
import re
import createcharacter

def look_northeast(game):
	temp= game.get("character").get("x")-1
	temp2=game.get("character").get("y")-1
	castlesize=game.get("castle").get("size")
	if temp<1:
		temp=castlesize
	if temp2<1:
		temp2=castlesize
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(temp2)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

def look_northwest(game):
	temp= game.get("character").get("x")-1
	temp2=game.get("character").get("y")+1
	castlesize=game.get("castle").get("size")
	if temp<1:
		temp=castlesize
	if temp2>castlesize:
		temp2=1
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(temp2)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

def look_southeast(game):
	temp= game.get("character").get("x")+1
	temp2=game.get("character").get("y")-1
	castlesize=game.get("castle").get("size")
	if temp>castlesize:
		temp=1
	if temp2<1:
		temp2=castlesize
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(temp2)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

def look_southwest(game):
	temp= game.get("character").get("x")+1
	temp2=game.get("character").get("y")+1
	castlesize=game.get("castle").get("size")
	if temp>castlesize:
		temp=1
	if temp2>castlesize:
		temp2=1
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(temp2)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

def look_north(game):
	temp=game.get("character").get("x")-1
	castlesize=game.get("castle").get("size")
	if temp<1:
		temp=castlesize
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

def look_south(game):
	temp=game.get("character").get("x")+1
	castlesize=game.get("castle").get("size")
	if temp>castlesize:
		temp=1
	game["character"]["moved"]=True
	X=str(temp)
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	
def look_east(game):
	temp=game.get("character").get("y")+1
	castlesize=game.get("castle").get("size")
	if temp>castlesize:
		temp=1
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(temp)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

def look_west(game):
	temp=game.get("character").get("y")-1
	castlesize=game.get("castle").get("size")
	if temp<1:
		temp=castlesize
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(temp)
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	


def go_north(game):
	game["character"]["x"]=game.get("character").get("x")-1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("x")<1:
		game["character"]["x"]=castlesize
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	

def go_south(game):
	game["character"]["x"]=game.get("character").get("x")+1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("x")>castlesize:
		game["character"]["x"]=1
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

	

def go_east(game):
	game["character"]["y"]=game.get("character").get("y")+1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("y")>castlesize:
		game["character"]["y"]=1
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

	

def go_west(game):
	game["character"]["y"]=game.get("character").get("y")-1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("y")<1:
		game["character"]["y"]=castlesize
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	

	

def go_sinkhole(game):

	print("; you fall to the next level!\n")

	game["character"]["z"]=game.get("character").get("z")-1
	castlesize=game.get("castle").get("size")
	if game.get("character").get("z")<1:
		game["character"]["z"]=castlesize
	game["character"]["moved"]=True
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	game["castle"][X][Y][Z]["explored"]=True	
	
	
def go_drink(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	if game.get("castle").get(X).get(Y).get(Z).get("contents").get("pool")==True:
		print("You drink from the pool and ", end="")
		drink=random.randint(1,9)
		if drink<7:
			print("feel ", end="")
			change=random.randint(1,3)
			if drink==1:
				print("smarter")
				game["character"]["intelligence"]=game["character"]["intelligence"]+change
			if drink==2:
				print("nimbler")
				game["character"]["dexterity"]=game["character"]["dexterity"]+change
			if drink==3:
				print("stronger")
				game["character"]["strength"]=game["character"]["strength"]+change
			if drink==4:
				print("dumber")
				game["character"]["intelligence"]=game["character"]["intelligence"]-change
			if drink==5:
				print("clumsier")
				game["character"]["intelligence"]=game["character"]["dexterity"]-change
			if drink==6:
				print("weaker")
				game["character"]["strength"]=game["character"]["strength"]-change
		else:
			if drink==7:
				print("become a ", end="")
				if game.get("character").get("sex")=="m":
					print("girl")
					game["character"]["sex"]="f"
				else:
					print("boy.")
					game["character"]["sex"]="m"
				
			if drink==8:
				temp=createcharacter.races
				list=[]
				for x in temp:
					list.append(temp.get(x).get("name"))
				race=random.choice(list)
				print("turn into a " + race)
				game["character"]["race"]=race
			if drink==9:
				print("nothing happens.")
				
	else:
		print("** If you want to drink, find a pool")
	
	
def go_map(game):

	print("\n")
	castlesize=game.get("castle").get("size")
	multiplier=(castlesize*13)+1
	level=str(game.get("character").get("z"))					

	for x in range(1,castlesize+1):
		X=str(x)
		if x==1:
			print("*"*multiplier)
		else:
			print("|\n" + "*"*multiplier)
		for y in range(1,castlesize+1):
			print("|", end="")
			Y=str(y)
			
			tempx=str(game.get("character").get("x"))
			tempy=str(game.get("character").get("y"))
			if tempx==X and tempy==Y:
				print("You are here", end="")
			else:
				if game.get("castle").get(X).get(Y).get(level).get("explored")==True:
					if game.get("castle").get(X).get(Y).get(level).get("contents")=={}:
						print(" "*12, end="")
					else:
						firstitem=""
						for contents in game.get("castle").get(X).get(Y).get(level).get("contents"):
							tempcontents=contents

							# Don't display gold if their aren't any (this case should never happen)
							if tempcontents=="gold":
								if game.get("castle").get(X).get(Y).get(level).get("contents").get("gold")==0:
									tempcontents=""
							# Same with flares
							elif tempcontents=="flares":
								if game.get("castle").get(X).get(Y).get(level).get("contents").get("flares")==0:
									tempcontents=""
							if not tempcontents=="":
								firstitem=tempcontents
								break
								
						length=len(firstitem)
						filler=11-length
						print(" " + firstitem, end="")
						print(" "*filler, end="")
							
				else:
					print(" " + "?"*10+" ", end='')
				
	print("|\n" + "*"*multiplier + "\n")
	
	

def go_flare(game):
	flares=game.get("character").get("flares")
	if flares>0:
		look_north(game)
		look_south(game)
		look_east(game)
		look_west(game)
		look_northeast(game)
		look_southeast(game)
		look_northwest(game)
		look_southwest(game)
		go_map(game)
		game["character"]["flares"]=game.get("chracter").get("flares")-1
	else:
		print("** Hey, bright one, you're out of flares!\n")
	

def go_lamp(game):
	regex=re.compile("[nsew]")
	exitloop=False
	if game.get("character").get("lamp")==True:
		print("Where do you want to shine the lamp (N,S,E,W)? ", end="")
		choice=input()[:1].lower()
		if re.match(regex, choice):
			if choice=="n":
				look_north(game)
			elif choice=="s":
				look_south(game)
			elif choice=="e":
				look_east(game)
			elif choice=="w":
				look_west(game)
		else:
			print("** That's not a direction, " + game.get("character").get("race") + "!")
	else:
		print("** You don't have a lamp, " + game.get("character").get("race") + "!")
	
	
def go_open_chest(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	contents=game.get("castle").get(X).get(Y).get(Z).get("contents").get("chest")
	
	print("contents of chest: ", contents)
	
	regex=re.compile("[yn]")
	if contents.get("opened")==False:
		if len(contents)>1:
			for content in contents:
				if content=="explodes":
					damage=random.randint(0,3)
					print("You open the chest and it explodes!!  You take " + str(damage) + " damage!!")
					game["character"]["strength"]=game["character"]["strength"]-damage
					del game["castle"][X][Y][Z]["contents"]["chest"]
					foundcontents=1
					break
				elif contents=="book":

					exittheloop=False
					while exittheloop==False:
						print("You found a book in the chest; do you want to open it?  ", end="")
						choice=input()
						if re.match(regex, choice):
							if choice=="y":
								tempcontents=contents.get("book")
								if contents=="strength":
									print("You found a manual of strength!!")
									character["strength"]=character["strength"]+18
								elif contents=="dexterity":
									print("You found a manual of dexterity!!")
									character["dexterity"]=character["dexterity"]+18
								elif contents=="intelligent":
									print("You found a manual of intelligence!!")
									character["dexterity"]=character["intelligence"]+18
								elif contents=="blind":
									print("A flash of light comes out of the book!!  Oh no, you are a blind" + game.get("character").get("race") + "!")
									character["blind"]=True
								elif contents=="stick":
									# Thought about saying you can't open a book with a book on your hand but that just seems awkward
									print("The book sticks to your hand, you are now unable to draw your weapon")
									character["bookstucktohand"]=True
								else:
									print(contents.get("book"))
								del game["castle"][X][Y][Z]["contents"]["chest"]["book"]
								exittheloop=True
							else:
								exittheloop=True
						else:
							print("** Invalid choice, yes or no")
				elif contents=="gold":
					print("You found gold!!!")
					gold=contents.get("gold")
					game["character"]["gold"]=game["character"]["gold"]+gold
				elif contents=="orb":
					print("You found an orb!!")
					game["castle"][X][Y][Z]["contents"]["orb"]=True
					del game["castle"][X][Y][Z]["contents"]["chest"]["orb"]	
		else:
			print("You opened the chest and ... it's empty")
			del game["castle"][X][Y][Z]["contents"]["chest"]
			
	

def go_open_book(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	
	if game.get("character").get("blind")==False:	
		#contents=roomcontents=game.get("castle").get(X).get(Y).get(Z).get("content").get("book")
		contents=game.get("castle").get(X).get(Y).get(Z).get("contents").get("book").get("title")
		if contents=="strength":
			print("\tYou found a manual of strength!!")
			game["character"]["strength"]=game["character"]["strength"]+18
		elif contents=="dexterity":
			print("\tYou found a manual of dexterity!!")
			game["character"]["dexterity"]=game["character"]["dexterity"]+18
		elif contents=="intelligence":
			print("\tYou found a manual of intelligence!!")
			game["character"]["intelligence"]=game["character"]["strength"]+18
		elif contents=="blind":
			print("\tA flash of light comes out of the book!!  Oh no, you are a blind" + game.get("character").get("race") + "!")
			character["blind"]=True
		elif contents=="sticks":
			# Thought about saying you can't open a book with a book on your hand but that just seems awkward
			print("\tThe book sticks to your hand, you are now unable to draw your weapon")
			character["bookstucktohand"]=True
		else:
			print("\t"+contents)
		del game["castle"][X][Y][Z]["contents"]["book"]
	else:
		print("** Blind " + game.get("character").get("race") + " can't read books!!")
	
	
def go_open(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	
	roomcontents=game.get("castle").get(X).get(Y).get(Z).get("contents")

	regex=re.compile("[bc]")
	
	
	if roomcontents.get("chest") or roomcontents.get("book"):
		if roomcontents.get("chest") and roomcontents.get("book"):
			regex=re.compile("[bc]")
			
			exittheloop=True
			while exittheloop==False:
				print("You found a book and a chest, which one do you want to open?  ", end="")
				choice=input().lower()[0]
			
				if re.match(choice):
					if choice=="b":
						go_open_book(game)
					else:
						go_open_chest(game)
					exittheloop=True
				else:
					print("\n** Your choices were to open a book or chest and you didn't select either.\n\n")
		if roomcontents.get("book"):
			go_open_book(game)
		else:
			go_open_chest(game)
	else:
		print("** Hey Genius, their is nothing to open!!")	
	

def go_gaze(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	castlesize=game.get("castle").get("size")

	if game.get("character").get("blind")==True:
		print("** Stupid " + game.get("character").get("race") + ", you are blind and can't gaze.")
	else:
		if game.get("castle").get(X).get(Y).get(Z).get("contents").get("orb")==True:
			print("You stare at the orb and see ", end="")
			randomdata=random.randint(0,9)
			if randomdata==0:
				# Find orb of zot
				for x in range(1, castlesize+1):
					A=str(x)
					for y in range(1, castlesize+1):
						B=str(y)
						for z in range(1,castlesize+1):
							C=str(z)
							if not game.get("castle").get(A).get(B).get(C).get("contents").get("warp")==None:
								if game.get("castle").get(A).get(B).get(C).get("contents").get("warp").get("orbofzot"):
									print("the orb of zot at location " + A +"/" + B + "/" + C)
			# Random Warp Stating Orb of Zot
			elif randomdata>=1 and randomdata<=4:
				datalist=[]
				for x in range(1, castlesize+1):
					A=str(x)
					for y in range(1, castlesize+1):
						B=str(y)
						for z in range(1,castlesize+1):
							C=str(z)
							if not game.get("castle").get(A).get(B).get(C).get("contents").get("warp")==None:
								datalist.append(A + "/" + B + "/" + C)
				print("the orb of zot at location " + random.choice(datalist))
			elif randomdata==5:
				print("a soap opera rerun")
			elif randomdata==6:
				print("yourself in a bloody heap")
				game["character"]["strength"]=game["character"]["strength"]-random.randint(1,3)
			elif randomdata==7:
				datalist=[]
				for x in range(1, castlesize+1):
					A=str(x)
					for y in range(1, castlesize+1):
						B=str(y)
						for z in range(1,castlesize+1):
							C=str(z)
							if not game.get("castle").get(A).get(B).get(C).get("contents").get("monster")==None:
								datalist.append(game.get("castle").get(A).get(B).get(C).get("contents").get("monster").get("name"))
				if datalist==[]:
					# Should only be the case if all monsters are dead
					print("nothing out of the ordinary but you feel like you are being watched")				
				else:
					choice=random.choice(datalist)
					if choice[0]=="a" or choice[0]=="e" or choice[0]=="i" or choice[0]=="o" or choice[0]=="u":
						print("an " + random.choice(datalist) + " staring back at you.")
					else:
						print("a " + random.choice(datalist) + " staring back at you.")
			elif randomdata==8:
				print("the world series playing")
			elif randomdata==9:
				print("your self ", end="")
				newdata=random.randint(0,1)
				if newdata==0:
					print("becoming a ", end="")
					if game.get("character").get("sex")=="m":
						print("girl", end="")
						game["character"]["sex"]="f"
					else:
						print("boy", end="")
						game["character"]["sex"]="m"
				else:				
					temp=createcharacter.races
					list=[]
					for x in temp:
						list.append(temp.get(x).get("name"))
					race=random.choice(list)
					print("turn into a " + race, end="")
					game["character"]["race"]=race
				print("; What kind of sorcery is this!!")
		else:
			print("** Their is no orb")
	

def go_teleport(game):
	regex="[0-9]+/[0-9]+/[0-9]"
	if game.get("character").get("runestaff")==True:
		print("Where do you want to teleport to (format is X/Y/Level, e.g. 1/3/5): ", end="")
		choice=input()
		if re.match(regex, choice):
			print("Choice was ", choice)
			data=choice.split("/")
			X=int(data[0])
			Y=int(data[1])
			Z=int(data[2])
			castlesize=game.get("castle").get("size")
			valid=True
			if X<1 or X>castlesize:
				print("X value is invalid")
				valid=False
			elif Y<1 or Y>castlesize:
				print("Y value is invalid")
				valid=False
			elif Z<1 or Z>castlesize:
				print("Level value is invalid")
				valid=False
			if valid==True:
				game["character"]["x"]=X
				game["character"]["y"]=Y
				game["character"]["z"]=Z
				game["castle"][str(X)][str(Y)][str(Z)]["explored"]=True
				if not game.get("castle").get(str(X)).get(str(Y)).get(str(Z)).get("contents").get("warp")==None:
					if game.get("castle").get(str(X)).get(str(Y)).get(str(Z)).get("contents").get("warp").get("orbofzot"):
						print("You just found ***THE ORB OF ZOT***!  The Rune Staff Disappeared")
						game["character"]["runestaff"] = False
						game["character"]["orbofzot"]  = True
						game["castle"][str(X)][str(Y)][str(Z)]["contents"]["warp"]["monster"]
						game["castle"][str(X)][str(Y)][str(Z)]["contents"]["monster"]=game["castle"][str(X)][str(Y)][str(Z)]["contents"]["warp"]["monster"]
						del game["castle"][str(X)][str(Y)][str(Z)]["contents"]["warp"]
						go_monster(game)
				
				

		else:
			print("** Invalid format; if you want to teleport; use the right format")
			

	else:
		print("** You can't teleport without the runestaff!")
	
	
def go_up(game):

	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	
	if not game.get("castle").get(X).get(Y).get(Z).get("contents").get("upstairs")==None:
	
		game["character"]["z"]=game.get("character").get("z")+1
		castlesize=game.get("castle").get("size")
		if game.get("character").get("z")>castlesize:
			game["character"]["z"]=1
		game["character"]["moved"]=True
		X=str(game.get("character").get("x"))
		Y=str(game.get("character").get("y"))
		Z=str(game.get("character").get("z"))
		game["castle"][X][Y][Z]["explored"]=True
	else:
		print("** There are no stairs going Up from here!")

	

def go_down(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	
	if not game.get("castle").get(X).get(Y).get(Z).get("contents").get("downstairs")==None:
		game["character"]["z"]=game.get("character").get("z")-1
		castlesize=game.get("castle").get("size")
		if game.get("character").get("z")<1:
			game["character"]["z"]=castlesize
		game["character"]["moved"]=True
		X=str(game.get("character").get("x"))
		Y=str(game.get("character").get("y"))
		Z=str(game.get("character").get("z"))
		game["castle"][X][Y][Z]["explored"]=True
	else:
		print("** There are no stairs going Down from here!")

	
	
	
def go_upstairs(game):
	print("; they go up")
	

def go_downstairs(game):
	print("; they go down")
	
	

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
				" a chicken egg!",
				" poop!"
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
				" a scream!  You think someone is being tortured!",
				" a rat!"
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

def go_warp(game):
	
	print("; you are being warped to a random location!!\n")
	castlesize=game.get("castle").get("size")
	X=random.randint(1,castlesize)
	Y=random.randint(1,castlesize)
	Z=random.randint(1,castlesize)
	
	print("You are warping to " + str(X) +"/"+str(Y)+"/"+str(Z))
	game["character"]["x"]=X
	game["character"]["y"]=Y
	game["character"]["z"]=Z
	game["castle"][str(X)][str(Y)][str(Z)]["explored"]=True		
	
	

def go_monster(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	
	
	# For some fluke of nature; this should never get called
	if game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster")==None:
		return
	else:
		#print(game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster"))
		asciiart=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("asciiart")
		monstername=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("name")
		monsterstrength=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("strength")
		monsterintelligence=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("intelligence")
		monsterdexterity=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("dexterity")
		if monsterdexterity<=0:
			monsterdexterity=1
		monsterattack=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("attack")
		monsterdefense=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("defense")
		monsterbreak=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("break")
		monsterbreaklie=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("breaklie")


		bribe=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("bribe")
		intelligence=game.get("character").get("intelligence")
		strength=game.get("character").get("strength")
		dexterity=game.get("character").get("dexterity")
		treasures=game.get("character").get("treasures")
		weapon=game.get("character").get("weapons").get("name")
		weaponeffect=game.get("character").get("weapons").get("effect")
		armor=game.get("character").get("armor").get("name")
		armoreffect=game.get("character").get("armor").get("effect")

		
		#print("You are at ( " + X + ", " + Y + " ) Level " + Z + "\n")
		print("Strength = " + str(strength), end="")
		print("	 Intelligence = " + str(intelligence), end="")
		print("	 Dexterity = " + str(dexterity))
		print("Treasures = " + str(len(game.get("character").get("treasures"))), end="")
		print(" Gold pieces = " + str(game.get("character").get("gold")))
		print("Weapons: " + str(game.get("character").get("weapons").get("name")) + "\tarmor: " + str(game.get("character").get("armor").get("name")))
		vowel=monstername[0].lower()
		print(asciiart)
		firststrike=False
		
		print("Using your keen eyes, you can see the monster has a strength of " + str(monsterstrength)  + " IQ of " + str(monsterintelligence) + " and Dexterity of " + str(monsterdexterity))
		print("Monster has an attack of " + str(monsterattack) + " and defense of " + str(monsterdefense))

		if monsterbreak==True:
			print("You can also see that the monster has wicked armor; your weapons and armor could break")
			
		
		
		for x in game.get("character").get("treasures"):
			if x=="Ruby red":
				firststrike=True
		if firststrike==False:
			role=dexterity+monsterdexterity
			tester=random.randint(1, role)
			if tester<=dexterity:
				firststrike=True
				
		exittheloop1=False

		# Attack, retreat, bribe, or spell
		monsterbribed=False
		retreated=False
		while exittheloop1==False:
			if vowel=="a" or vowel=="e" or vowel=="i" or vowel=="o" or vowel=="u":
				print("\nYou are fighting an " + monstername)
			else:
				print("\nYou are fighting a " + monstername)
			if firststrike==True:
				exittheloop2=False
				while exittheloop2==False:
					if intelligence >=15:
						if bribe==True:
							print("You may attack, retreat, bribe the monster, or cast spell")
							choices="[arbs]"
						else:
							print("You may attack, retreat, or cast spell")
							choices="[ars]"
					else:
						if bribe==True:
							print("You may attack, retreat, or bribe the monster")
							choices="[arb]"
						else:
							print("You may attack, or retreat")
							choices="[ar]"
					choices=re.compile(choices)
					print("What is your choice:  ", end="")
					choice=input()
					if re.match(choices, choice):
						if choice=="b":
							treasure=None
							if len(treasures)>0:
								randomdata=random.randint(0, treasures-1)
								treasure=treasures[randomdata]
							if not treasure==None:
								regex="[yn]"
								exittheloop3=False
								while exittheloop3==False:	
									print("The monster says give me the treasure " + treasure + " and I'll let you go")
									print("Do you agree: ", end="")
									choice=input()[0].lower()
									if re.match(regex, choice):
										if choice=="y":
											game["character"]["treasures"].remove(treasure)
											castle[X][Y][Z]["contents"][content]["treasures"].append(treasure)
											monsterbribed=True
										exittheloop3=True
									else:
										print("** Valid choices are yes or no!!")
							if monsterbribed==False:
								randomdata=random.randint(100, 2000)
								if randomdata>=game.get("character").get("gold"):
									exittheloop3=False
									while exittheloop3==True:
										print("Give me " + str(randomdata) + " gold pieces and I'll let you live")
										regex="[yn]"
										print("Do you agree: ", end="")
										choice=input()[0].lower()
										if re.match(regex, choice):
											if choice=="y":
												monsterbribed=True
												game["character"]["gold"]=game["character"]["gold"]-randomdata
												game["castle"][X][Y][Z]["contents"]["monster"]["horde"]=game["castle"][X][Y][Z]["contents"]["monster"]["horde"]+randomdata
											exittheloop3=True
										else:
											print("** Valid choices are yes or no!!")
									
								else:
									print("** The only thing you I want is your life!!!")
									game["castle"][X][Y][Z]["contents"]["monster"]["bribe"]=False
									bribe=False																	
												
						elif choice=="a":
							print("\nAttacking monster!!\n")
							if weaponeffect==0:
								print("** What are you going to do?  Beat it with your hands?")
							elif  game.get("character").get("bookstucktohand")==True:
								print("** You have a book stuck to your hand.  What are you going to do?  Beat it with a book?")
							else:	
								role=dexterity+monsterdexterity
								tester=random.randint(1, role)- int(intelligence/4) - int(strength/4)
								if tester<=dexterity:
									maxhits=int(dexterity/monsterdexterity)
									if maxhits<=1:
										hits=1
									else:
										hits=random.randint(1, maxhits)
										
									print("You did " + str(hits) + " hits!!")
									for hit in range(0, hits):
										damage=random.randint(weaponeffect/2,weaponeffect)
										defense=random.randint(int(monsterdefense/2),int(monsterdefense))
										# 22 sided die; roll 0 for critical
										critical=random.randint(0,21)
										if critical==0:
											print("You did a critical hit; critical hits ignore armor and does twice the damge")
											print("\tYou did " + str(damage*2) + " damage to the monster")
											game["castle"][X][Y][Z]["contents"]["monster"]["strength"]=game["castle"][X][Y][Z]["contents"]["monster"]["strength"] - damage*2
										else:
											'''monsterroles=[]
											for x in range(1, len(monsterdefense)):
												monsterroles.append(random.randint(1,6))
											humanroles=[]
											for x in range(1, len(weaponeffect)):
												humanroles.append(random.randint(1,6))
											for monsterrole, humanrole in mosteroles.sorted(), humanroles().sorted():'''
											

											if damage>defense:
												danger=damage-defense

												print("\tYou did " + str(danger) + " damage to the monster")
												game["castle"][X][Y][Z]["contents"]["monster"]["strength"]=game["castle"][X][Y][Z]["contents"]["monster"]["strength"] - danger
											else:
												print("\tYou did no damage to the monster")
									if monsterbreak==True and hits>0:
										diditbreak=random.randint(0, monsterbreaklie)
										if diditbreak==0:
											print("** Oh no, your " + game["character"]["weapons"]["name"] + " broke!!")
											game["character"]["weapons"]["name"]="Nothing"
											game["character"]["weapons"]["effect"]=0

								else:
									print("** You missed the monster **")
						elif choice=="r":
							role=dexterity+monsterdexterity
							tester=random.randint(1, role)
							if tester<=dexterity:
								print("You ran for your life!!")
								exittheloop1=True
								retreated=True
							else:
								print("You can't get away")

						elif choice=="s":
							print("Casting Spell")
						exittheloop2=True
						firststrike=True
					else:
						print("** Invalid choice; try again")
			monsterdead=0
			if game["castle"][X][Y][Z]["contents"]["monster"]["strength"]<=0:
				print("\nMonster died from lack of Strength")
				monsterdead=1
			elif game["castle"][X][Y][Z]["contents"]["monster"]["intelligence"]<=0:
				print("\nMonster died from stupidity")
				monsterdead=1
			elif game["castle"][X][Y][Z]["contents"]["monster"]["dexterity"]<=0:
				print("\nMonster stopped moving (died from lack of dexterity)")
				monsterdead=1
			if monsterdead==1:
				horde=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("horde")
				print("You find the monsters horde of " + str(horde) + " gold coins; they are now yours")
				game["character"]["gold"]=game["character"]["gold"]+horde
				if game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("runestaff")==True:
					game["character"]["runestaff"]=True
					print("Great Zot!  You found the Rune Staff!")
				del game["castle"][X][Y][Z]["contents"]["monster"]
				exittheloop1=True
				break
			if monsterdead==0 and monsterbribed==False and retreated==False:
				role=dexterity+monsterdexterity
				tester=random.randint(1, role)- int(monsterintelligence/4) - int(monsterstrength/4)
				if tester<=monsterdexterity:
					maxhits=int(monsterdexterity/dexterity)
					
					if maxhits<=1:
						hits=1
					else:
						hits=random.randint(1, maxhits)
					print("\nDefending again monster;\nMonster did " + str(hits) + " hits!!")
					for hit in range(0, hits):
						damage=random.randint(int(monsterattack/2),monsterattack)
						defense=random.randint(int(armoreffect/2),armoreffect)
						#print("monsterattack", monsterattack, damage, defense)

						if damage>defense:
							danger=damage-defense
							print("\tThe monster did " + str(danger) + " damage.")
							game["character"]["strength"]=game["character"]["strength"]-danger
						else:
							print("\tThe monster did no damage")
					if monsterbreak==True and hits>0:
						diditbreak=random.randint(0, monsterbreaklie)
						if diditbreak==0:
							print("** Oh no, the monster broke your " + game["character"]["armor"]["name"])
							game["character"]["armor"]["name"]="Nothing"
							game["character"]["armor"]["effect"]=0

				else:
					print("** The monster missed you")

			if game.get("character").get("strength")<=0 or game.get("character").get("dexterity")<=0 or game.get("character").get("intelligence")<=0:
				
				print("The monster killed you!!")
				#Character is dead
				exittheloop1=True

				
			firststrike=True
				
					
			
	
	

def go_vendor(game):
	# Add logic to interact with vendor
	print("\nYou found a vendor!!\n")
	regex="[tai]"
	exittheloop=False
	while exittheloop==False:
		print("Current Inventory:")
		print("\t" + game.get("character").get("weapons").get("name") + " and " + game.get("character").get("armor").get("name"))
		print("\nYou also have " + str(game.get("character").get("flares")) + " flares and " + str(game.get("character").get("gold")) + " gold pieces\n")
		if len(game.get("character").get("treasures"))>0:
			print("Treasures:")
			for x in game.get("character").get("treasures"):
				print("\tx")

		if game.get("character").get("gold")>=20 or len(game.get("character").get("treasures"))>0:
			print("You may Trade with, Attack, Or Ignore the Vendor: ", end="")
			regex="[tai]"
		else:
			print("You may Attack or ignore the vendor: ", end="")
			regex="[ai]"

		choice=input()[0].lower()
		if re.match(regex, choice):
			if choice=="a":
				print("Attacking Vendor")
			elif choice=="t":
				armorregex="[nlcp]"
				buffer="Nothing<0>  Leather<1250>  Chainmail<1500>  Plate<2000>"
				exittheloop2=False
				while exittheloop2==False:
					print(buffer)
					print("Choice: ", end="")
					choice2=input()[0].lower()
					if re.match(armorregex, choice2):
						if choice=="n":
							exittheloop2=True
						elif choice=="l":
							if game["character"]["gold"]>=1250:
								if game["character"]["armor"]["effect"]<2:
									game["character"]["gold"]=game["character"]["gold"]-1250
									game["character"]["armor"]["name"]="Leather"
									game["character"]["armor"]["effect"]=2
									exittheloop2=True
								else:
									print("** Your current armor is better")
							else:
								print("** You cant' afford that")
						elif choice=="c":
							if game["character"]["gold"]>=1500:
								if game["character"]["armor"]["effect"]<4:
									game["character"]["gold"]=game["character"]["gold"]-1500
									game["character"]["armor"]["name"]="Chainmail"
									game["character"]["armor"]["effect"]=4
									exittheloop2=True
								else:
									print("** Your current armor is better")
							else:
								print("** You can't afford that")
								
						elif choice=="p":
							if game["character"]["gold"]>=2000:
								if game["character"]["armor"]["effect"]<6:
									game["character"]["gold"]=game["character"]["gold"]-2000
									game["character"]["armor"]["name"]="Plate"
									game["character"]["armor"]["effect"]=6
									exittheloop2=True
								else:
									print("** Your current armor is better")
							else:
								print("** You can't affort that")
					else:
						print("** Invalid choice")							
				weaponregex="[ndms]"
				buffer="Nothing<0>  Dagger<1250>  Mace<1500>  Sword<2000>"
				exittheloop2=False
				while exittheloop2==False:
					print("Choice: ", end="")
					choice2=input()[0].lower()
					if re.match(weaponregex, choice2):
						if choice=="n":
							exittheloop2=True
						elif choice=="l":
							if game["character"]["gold"]>=1250:
								if game["character"]["weapon"]["effect"]<2:
									game["character"]["gold"]=game["character"]["gold"]-1250
									game["character"]["weapon"]["name"]="Dagger"
									game["character"]["weapon"]["effect"]=2
									exittheloop2=True
								else:
									print("** Your current armor is better")
							else:
								print("** You can't affort that")
						elif choice=="c":
							if game["character"]["gold"]>=1500:
								if game["character"]["weapon"]["effect"]<4:
									game["character"]["gold"]=game["character"]["gold"]-1500
									game["character"]["weapon"]["name"]="Mace"
									game["character"]["weapon"]["effect"]=4
									exittheloop2=True
								else:
									print("** Your current armor is better")
							else:
								print("** You can't affort that")
						elif choice=="p":
							if game["character"]["gold"]>=2000:
								if game["character"]["weapon"]["effect"]<6:
									game["character"]["gold"]=game["character"]["gold"]-2000
									game["character"]["weapon"]["name"]="Sword"
									game["character"]["weapon"]["effect"]=6
									exittheloop2=True
								else:
									print("** Your current armor is better")
							else:
								print("** You can't affort that")
						exittheloop2=True
					else:
						print("** Invalid choice")							
				if game.get("character").get("gold")>=1000:
					exiththeloop2=False
					regex="[yn]"
					while exittheloop==False:
						print("Do you want to buy a strength potion for 1000 gps: ")
						choice=input().lower()[0]
						if re.match(regex, choice):
					 		if choice=="y":
					 			game["character"]["gold"]=game["character"]["gold"]-1000
					 			if game.get("character").get("gold")<1000:
					 				exittheloop=1
					 			strength=random.randint(1,5)
					 			game["character"]["strength"]=game["character"]["strength"]+strength
					 			print("Your strength is now " + str(game["character"]["strength"]))
					 		else:
					 			exittheloop=1
				if game.get("character").get("gold")>=1000:
					exiththeloop2=False
					regex="[yn]"
					while exittheloop==False:
						print("Do you want to buy a dexterity potion for 1000 gps: ")
						choice=input().lower()[0]
						if re.match(regex, choice):
							if choice=="y":
								game["character"]["gold"]=game["character"]["gold"]-1000
								if game.get("character").get("gold")<1000:
									exittheloop=1
								dexterity=random.randint(1,5)
								game["character"]["dexterity"]=game["character"]["dexterity"]+dexterity
								print("Your dexterity is now " + str(game["character"]["dexterity"]))
							else:
					 			exittheloop=1
				if game.get("character").get("gold")>=1000:
					exiththeloop2=False
					regex="[yn]"
					while exittheloop==False:
						print("Do you want to buy an intelligence potion for 1000 gps: ")
						choice=input().lower()[0]
						if re.match(regex, choice):
							if choice=="y":
								game["character"]["gold"]=game["character"]["gold"]-1000
								if game.get("character").get("gold")<1000:
									exittheloop=1
								intelligence=random.randint(1,5)
								game["character"]["intelligence"]=game["character"]["intelligence"]+intelligence
								print("Your intelligence is now " + str(game["character"]["intelligence"]))
							else:
								exittheloop=1
					 			

					 					
				if game.get("character").get("gold")>=1000 and game["character"]["lamp"]==False:
					print("Do you want to buy a lamp for 1000 gps:  ", end="")
					exiththeloop2=False
					lampregex="[yn]"
					while exittheloop2==False:
						choice=input()[0]
						if choice=="n":
							exittheloop2=True
						else:
							game["character"]["lamp"]=True
							game["character"]["gold"]=game["character"]["gold"]-1000
							exittheloop2=True
				if game.get("character").get("gold")>=20:
					exittheloop2=False
					while exittheloop2==False:
						print("Ok, " + game.get("character").get("race") + ", you may buy flares for 20 gps each; how many do you want?  ", end="")
						flareregex="[0-9]+"
						choice=input()
						if re.match(flareregex, choice):
							flares=int(choice)
							price=flares*20
							if price>game["character"]["gold"]:
								print("** You can only afford " + str(int(game["character"]["gold"]/20)) + " flares!!")
							else:
								if not flares==0:
									game["character"]["gold"]=game["character"]["gold"]-price
									game["character"]["flares"]=game["character"]["flares"]+flares
								exittheloop2=True
								
						else:
							print("** Invalid Choice")						
				
			else: 
				exittheloop=1

	
	
def go_chest(game):
	print("; you may open it or leave it.  It may contain gold, books, or it might be boobie trapped.")
	

def go_pool(game):
	print("; you may drink from the water.")
	

def go_book(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))

	print("; You may open it or leave it.")
	
	if not game.get("castle").get(X).get(Y).get(Z).get("contents").get("book").get("treasure")==None:
		if game.get("character").get("blind")==True:
			print("You rub your hands on the book.")	
			
		if len(game.get("castle").get(X).get(Y).get(Z).get("contents").get("book").get("treasure"))==1:
			print("\nTheir is a jewel on the book; you pry it off and find it's treasure.")
		else:
			# I doubt this ever happens but it could
			print("\nTheir are jewels on the book; you pry them off and find treasure.")
		for treasure in game.get("castle").get(X).get(Y).get(Z).get("contents").get("book").get("treasure"):
			print("\tYou found: " + treasure + "it's now yours.")
			game["character"]["treasure"].append(treasure)
		del game["castle"][X][Y][Z]["contents"]["book"]["treasure"]
	
	

def go_orb(game):
	if game.get("character").get("blind")==True:
		print("; you are blind and can't gaze; you must find or buy the opal eye before you are able to gaze.")	
	else:
		print("; gaze at your own peril")
	

def do_nothing(game):
	return
	
def go_entrance(game):
	print("; go north to leave the game")
	

def go_gold(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	
	goldamount=game.get("castle").get(X).get(Y).get(Z).get("contents").get("gold")
	if goldamount>0:
		if goldamount>1:
			print("\n\nYou found " + str(goldamount) + " gold pieces; they are now yours.\n")
		else:
			print("\nnYou found a miserly single gold piece\n")
		game["character"]["gold"]=game["character"]["gold"]+goldamount

		#Can't delete during iteration so just setting to zero; deleting if you use map
		#game["castle"][X][Y][Z]["contents"]["gold"]=0
		del game["castle"][X][Y][Z]["contents"]["gold"]
	

def go_flares(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	flaresamount=game.get("castle").get(X).get(Y).get(Z).get("contents").get("flares")

	if flaresamount>0:
		if flaresamount>1:
			print("\n\nYou found " + str(flaresamount) + " flares; they are now yours.\n")
		else:
			print("\n\nYou found a flare.  It is now yours.\n")
			
		game["character"]["gold"]=game["character"]["flares"]+flaresamount

		del game["castle"][X][Y][Z]["contents"]["flares"]
		
	
	
def go_treasure(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	print("\n\nYou found treasure: ")
	for treasure in game.get("castle").get(X).get(Y).get(Z).get("contents").get("treasures"):
		print("\tThe treasure " + treasure + " was added to your inventory")
		game["character"]["treasures"].append(treasure)
	del game["castle"][X][Y][Z]["contents"]["treasures"]
	
	
def action_room(game):
	actiondict={
		"warp":  go_warp,
		"monster": go_monster,
		"sinkhole": go_sinkhole,
		"entrance": go_entrance,
		"vendor": go_vendor,
		"chest": go_chest,
		"pool": go_pool,
		"book": go_book,
		"orb": go_orb,
		"upstairs": go_upstairs,
		"downstairs": go_downstairs,
		"gold": go_gold,
		"flares": go_flares,
		"treasures": go_treasure
	}

	if game.get("character").get("moved")==True:
		X=str(game.get("character").get("x"))
		Y=str(game.get("character").get("y"))
		Z=str(game.get("character").get("z"))
		print("Your current position is: " + X +"/"+Y+"/"+Z)
		game["character"]["moved"]=False

		if not game.get("castle").get(X).get(Y).get(Z).get("contents")=={}:
			datacontents=[]
			for contents in game.get("castle").get(X).get(Y).get(Z).get("contents"):
				datacontents.append(contents)
			counter=0	
			for contents in datacontents:
				if contents=="monster":
					counter=counter+1
					actiondict.get(contents)(game)
					break
				else:
					if counter==0:
						print("\nIn this room, you find:")
					print("\t"+contents.capitalize(), end="")
					actiondict.get(contents)(game)
					counter=counter+1
		else:
			print("\tNothing.  The room is empty.")
		randint=random.randint(0,3)
		
		if randint==0:
			ambience()
			
	

def castle_help(game):

	print(""""*** WIZARD'S CASTLE COMMAND AND INFORMATION SUMMARY ***

The following commands are available :

H/elp	  N/orth	S/outh	  E/ast		W/est	  U/p
D/own	  DR/ink	M/ap	  F/lare	L/amp	  O/pen
G/aze	  T/eleport Q/uit

the contents of rooms are as follows :"

""          = empty room       book     = book               chest    = chest
downstairs  = stairs down      entrance = entrance/exit      flares   = flares
gold        = gold pieces      monster  = monster            orb      = crystal orb
pool        = magic pool       sinkhole = sinkhole           treasure = treasure
upstairs    = stairs up	       vendor   = vendor             warp     = warp/orb

The benefits of having treasures are :

Ruby red - avoid lethargy     Pale pearl - avoid leech
Green gem - avoid forgetting  Opal eye - cures blindness
Blue flame - dissolves books  Norn stone - no benefit
Palantir - no benefit	      Silmaril - no benefit

Additionally:  

	The Ruby Red Gem gives you first strike and prevents losing Dexterity from pools or gazing
	The Green Gem prevents you from your IQ decreasing from pools and gazing
	The Pale Pearl prevents you from losing strength from pools and gazing

Press return when ready to resume""")

	input()
	
	

