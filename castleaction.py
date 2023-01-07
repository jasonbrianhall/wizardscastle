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
						for contents in game.get("castle").get(X).get(Y).get(level).get("contents"):
							length=len(contents)
							filler=11-length
							print(" " + contents, end="")
							print(" "*filler, end="")
							
							break
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
	return game

def go_open_book(game):
	return game

	
def go_open(game):
	X=str(game.get("character").get("x"))
	Y=str(game.get("character").get("y"))
	Z=str(game.get("character").get("z"))
	
	roomcontents=game.get("castle").get(X).get(Y).get(Z).get("contents")
	
	if roomcontents.get("chest") or roomcontents.get("book"):
		if roomcontents.get("chest") and roomcontents.get("book"):
			regex=re.compile("[bc]")
			print("You found a book and a chest, which one do you want to open?  ", end="")
			choice=input().lower()[0]
			if re.match("choice"):
				if choice=="b":
					game=go_open_book(game)
				else:
					game=go_open_chest(game)
			else:
				print("** Your choices were a book or chest and you didn't select either.")
		if roomcontents.get("book"):
			game=go_open_book(game)
		else:
			game=go_open_chest(game)
	else:
		print("** Hey Genius, their is nothing to open!!")	
	return game

def go_gaze(game):
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
	# Add logic to fight monster
	print("\nYou found a monster!!\n")
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
	print("; You may open it or leave it.")
	return game

def go_orb(game):
	print("; gaze at your own peril")
	return game

def do_nothing(game):
	return game
	
def go_entrance(game):
	print("; go north to leave the game")

def action_room(game):
	actiondict={
		"warp":  go_warp,
		"monster": go_monster,
		"sinkhole": go_sinkhole,
		"entrance": do_nothing,
		"vendor": go_vendor,
		"chest": go_chest,
		"pool": go_pool,
		"book": go_book,
		"orb": go_orb,
		"upstairs": go_upstairs,
		"downstairs": go_downstairs
	}

	if game.get("character").get("moved")==True:
		X=str(game.get("character").get("x"))
		Y=str(game.get("character").get("y"))
		Z=str(game.get("character").get("z"))
		print("Your current position is: " + X +"/"+Y+"/"+Z)
		game["character"]["moved"]=False

		if not game.get("castle").get(X).get(Y).get(Z).get("contents")=={}:
			print("\nIn this room, you find:")
			for contents in game.get("castle").get(X).get(Y).get(Z).get("contents"):
				print("\t"+contents.capitalize(), end="")
				game=actiondict.get(contents)(game)
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

Ruby red - avoid lethargy	  Pale pearl - avoid leech
Green gem - avoid forgetting  Opal eye - cures blindness
Blue flame - dissolves books  Norn stone - no benefit
Palantir - no benefit		  Silmaril - no benefit

Press return when ready to resume""")

	input()
	
	return game

