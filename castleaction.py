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
	return game

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
	return game

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
	return game

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
	return game



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
	return game

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
	return game
	
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
	return game

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
	return game


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
	return game

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

	return game

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

	return game

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

	return game

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
	return game
	
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
	return game
	
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
	
	return game

def go_flare(game):
	flares=game.get("character").get("flares")
	if flares>0:
		game=look_north(game)
		game=look_south(game)
		game=look_east(game)
		game=look_west(game)
		game=look_northeast(game)
		game=look_southeast(game)
		game=look_northwest(game)
		game=look_southwest(game)
		go_map(game)
		game["character"]["flares"]=game.get("chracter").get("flares")-1
	else:
		print("** Hey, bright one, you're out of flares!\n")
	return game

def go_lamp(game):
	regex=re.compile("[nsew]")
	exitloop=False
	if game.get("character").get("lamp")==True:
		print("Where do you want to shine the lamp (N,S,E,W)? ", end="")
		choice=input()[:1].lower()
		if re.match(regex, choice):
			if choice=="n":
				game=look_north(game)
			elif choice=="s":
				game=look_south(game)
			elif choice=="e":
				game=look_east(game)
			elif choice=="w":
				game=look_west(game)
		else:
			print("** That's not a direction, " + game.get("character").get("race") + "!")
	else:
		print("** You don't have a lamp, " + game.get("character").get("race") + "!")
	return game
	
def go_open_chest(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	contents=game.get("castle").get(X).get(Y).get(Z).get("contents").get("chest")
	
	print("contents of chest: ", contents)
	
	regex=re.compile("[yn]")
	if contents.get("opened")==False:
		if contents.get("book"):

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
	return game

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
		elif contents=="stick":
			# Thought about saying you can't open a book with a book on your hand but that just seems awkward
			print("\tThe book sticks to your hand, you are now unable to draw your weapon")
			character["bookstucktohand"]=True
		else:
			print("\t"+contents)
		del game["castle"][X][Y][Z]["contents"]["book"]
	else:
		print("** Blind " + game.get("character").get("race") + " can't read books!!")
	return game
	
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
						game=go_open_book(game)
					else:
						game=go_open_chest(game)
					exittheloop=True
				else:
					print("\n** Your choices were to open a book or chest and you didn't select either.\n\n")
		if roomcontents.get("book"):
			game=go_open_book(game)
		else:
			game=go_open_chest(game)
	else:
		print("** Hey Genius, their is nothing to open!!")	
	return game

def go_gaze(game):
	if game.get("character").get("blind")==True:
		print("** Stupid " + game.get("character").get("race") + ", you are blind and can't gaze.")
	else:
		print("Add logic for gazing here")
	return game

def go_teleport(game):
	return game
	
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

	return game

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

	return game
	
	
def go_upstairs(game):
	print("; they go up")
	return game

def go_downstairs(game):
	print("; they go down")
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
	
	return game

def go_monster(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	
	
	# For some fluke of nature; this should never get called
	if game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster")==None:
		return game
	else:
		#print(game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster"))
		asciiart=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("asciiart")
		monstername=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("name")
		monsterstrength=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("strength")
		monsterintelligence=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("intelligence")
		monsterdexterity=game.get("castle").get(X).get(Y).get(Z).get("contents").get("monster").get("dexterity")
		
		intelligence=game.get("character").get("intelligence")
		strength=game.get("character").get("strength")
		dexterity=game.get("character").get("dexterity")
		print("You are at ( " + X + ", " + Y + " ) Level " + Z + "\n")
		print("Strength = " + str(strength), end="")
		print("	 Intelligence = " + str(intelligence), end="")
		print("	 Dexterity = " + str(dexterity))
		print("Treasures = " + str(len(game.get("character").get("treasures"))), end="")
		print(" Gold pieces = " + str(game.get("character").get("gold")))
		print("Weapons: " + str(game.get("character").get("weapons").get("name")) + "\tarmor: " + str(game.get("character").get("armor").get("name")))
		vowel=monstername[0].lower()
		print(asciiart)
		firststrike=False
		for x in game.get("character").get("treasures"):
			if x=="Ruby red":
				firststrike=True
		if firststrike==False:
			role=dexterity+monsterdexterity
			tester=random.randint(1, role)
			if tester<=role:
				firststrike=True
				
		exittheloop=False

		# Attack, retreat, bribe, or spell
		choices="[arbs]"
		while exittheloop==False:
			if vowel=="a" or vowel=="e" or vowel=="i" or vowel=="o" or vowel=="u":
				print("\nYou are fighting an " + monstername)
			else:
				print("\nYou are fighting a " + monstername)
			if firststrike==True:
				if intelligence >=15:
					print("You may attack, retreat, bribe, or cast spell")
					
			
	
		return game

def go_vendor(game):
	# Add logic to interact with vendor
	print("\nYou found a vendor!!\n")
	return game
	
def go_chest(game):
	print("; you may open it or leave it.  It may contain gold, books, or it might be boobie trapped.")
	return game

def go_pool(game):
	print("; you may drink from the water.")
	return game

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
	
	return game

def go_orb(game):
	if game.get("character").get("blind")==True:
		print("; you are blind and can't gaze; you must find or buy the opal eye before you are able to gaze.")	
	else:
		print("; gaze at your own peril")
	return game

def do_nothing(game):
	return game
	
def go_entrance(game):
	print("; go north to leave the game")
	return game

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
	return game

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
		
	return game
	
def go_treasure(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	print("\n\nYou found treasure: ")
	for treasure in game.get("castle").get(X).get(Y).get(Z).get("contents").get("treasure"):
		print("\tThe treasure " + treasure + " was added to your inventory")
		game["character"]["treasure"].append(treasure)
	del game["castle"][X][Y][Z]["contents"]["treasure"]
	return game
	
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
		"treasure": go_treasure
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
				else:
					if counter==0:
						print("\nIn this room, you find:")
					print("\t"+contents.capitalize(), end="")
					counter=counter+1
				game=actiondict.get(contents)(game)
				if contents=="monster":
					break
		else:
			print("\tNothing.  The room is empty.")
		randint=random.randint(0,3)
		if randint==0:
			ambience()
			
	return game

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

	The Ruby Red Gem gives you first strike or losing Dexterity from pools or gazing
	The Green Gem prevents you from your IQ decreasing from pools and gazing
	The Pale Pearl prevents you from losing strength from pools and gazing

Press return when ready to resume""")

	input()
	
	return game

