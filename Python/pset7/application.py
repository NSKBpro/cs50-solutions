from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for, jsonify
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
import time

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    # Select query to get current users cash
    rows = db.execute("SELECT * FROM shares WHERE userid = :id", id=session["user_id"])
    tableRows = [dict() for x in range(len(rows))]
    i = 0
    total = 0
    for row in rows:
        tableRows[i] = {
            'symbol':row['symbol'],
            'name':lookup(row['symbol']).get('name'),
            'shares':row['shares'],
            'price':lookup(row['symbol']).get('price'),
            'total':usd(lookup(row['symbol']).get('price') * float(row['shares']))
        }
        total = total + lookup(row['symbol']).get('price') * float(row['shares'])
        i = i+1
    if request.args.get('justTotal'):
        return jsonify(total=usd(total))
    else:    
        return render_template("index.html",tableRows=tableRows,total=usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        formSymbol=request.form['symbol'].upper()
        formShares=request.form['shares']
        
        if not formSymbol:
            #rendering apology because frontend already has full validation for this
            return apology("Check share symbol")
        elif not formShares:
            #rendering apology because frontend already has full validation for this
            return apology("Check amount of shares to buy")
        
        if lookup(formSymbol) == None:
            #Return json to frontend (will handle it on frontend)
            return jsonify(success=False,message="Error getting share price, check share symbol.")
            
        sharesPrice = lookup(formSymbol).get('price') * float(formShares)
        
        # Select query to get current users cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        currentCash=rows[0]['cash']
        if currentCash - sharesPrice > 0:    
            if sharesPrice != 0:
                rows = db.execute("SELECT shares FROM shares WHERE userid = :id AND symbol = :symbol", id=session["user_id"], symbol=formSymbol)
                print (len(rows))
                if len(rows) == 0:
                    # Query to insert NEW share to database
                    resultInsert = db.execute("INSERT INTO shares (userid,symbol,shares) VALUES (:userid,:symbol,:shares)", userid=session["user_id"], symbol=formSymbol, shares= formShares)
                else:
                    # Query to update existing share in database
                    resultInsert = db.execute("UPDATE shares SET shares=shares+:shares WHERE userid = :userid AND symbol = :symbol", userid=session["user_id"], symbol=formSymbol, shares= formShares)                   
                   
                # update query to change users cash
                resultUpdate = db.execute("UPDATE users SET cash=cash-" + str(sharesPrice) +" WHERE id=" + str(session["user_id"]))
                #Checking if both queries were successful
                if resultInsert != 0 and resultUpdate != 0:
                    #Insert row to history table
                    db.execute("INSERT INTO history (userid,symbol,shares,price,kind,date) VALUES (:userid,:symbol,:shares,:price,:kind,:date)",userid=session["user_id"],symbol=formSymbol,shares=formShares,price=usd(lookup(formSymbol).get('price')),kind="BUY",date=time.strftime('%Y-%m-%d %H:%M:%S'))
                    #Get updated shares and total
                    check = db.execute("SELECT shares FROM shares WHERE userid = :userid AND symbol = :symbol", userid=session["user_id"], symbol=formSymbol)
                    shares = int(check[0]['shares'])
                    total=lookup(formSymbol).get('price') * shares
                    #Return json to frontend (will handle it on frontend)
                    return jsonify(success=True,total=usd(total))
                else:
                    #Return json to frontend (will handle it on frontend)
                    return jsonify(success=False,message="Error while making Database changes.")
        else:
            #Return json to frontend (will handle it on frontend)
            return jsonify(success=False,message="You don't have enough cash to buy these shares.")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    # Select query to get current users cash
    rows = db.execute("SELECT * FROM history WHERE userid = :id ORDER BY date DESC", id=session["user_id"])
    tableRows = [dict() for x in range(len(rows))]
    i = 0
    for row in rows:
        tableRows[i] = {
            'symbol':row['symbol'],
            'name':lookup(row['symbol']).get('name'),
            'shares':row['shares'],
            'price':lookup(row['symbol']).get('price'),
            'kind':row['kind'],
            'date':row['date']
        }
        i = i+1
        
    return render_template("history.html",tableRows=tableRows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/forgotpw", methods=["GET", "POST"])
def forgotpw():
    """Log user in."""
    
    if request.method == "GET":
        return render_template("forgotpw.html")
    else:
        username = request.form['username']
        password = request.form['password']
        
        # ensure username was submitted
        if not username:
            return apology("must provide username")
        # ensure password was submitted
        elif not password:
            return apology("must provide password")  
        
         # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)
        if len(rows) == 1:
            # insert query to change password
            result = db.execute("UPDATE users SET hash=:password WHERE id=:id", password=pwd_context.encrypt(password), id=rows[0]["id"])
            if result != 0:
                # remember which user has logged in
                session["user_id"] = rows[0]["id"]
                # redirect user to home page
                return redirect(url_for("index"))
            else:
                return apology("Error while changing password")
        else:
            return apology("Cannot find user with username " + username)

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # Render quote.html if GET request is sent 
    if request.method == "GET":
        return render_template("quote.html")
    
    # Handle form data and return result (Will use AJAX on frontend)    
    elif request.method == "POST": 
        #Lookup for stock using helper method (Getting post request from client)
        resultDict = lookup(request.form['symbol'])
        #Return JSON so we can parse it easily on front end, not rendering page and using ajax instead helps with UX
        return jsonify(name=resultDict.get('name'),price=usd(resultDict.get('price')),symbol=resultDict.get('symbol'))

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")
        # ensure username is unique    
        elif len(db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))) != 0:
            return apology("username already taken");

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
        
        # ensure passwords match
        elif not request.form.get("password") == request.form.get("passwordRepeat"):
            return apology("passwords don't match")        
    
        # insert query to register user
        result = db.execute("INSERT INTO users (username,hash) VALUES (:username,:password)", username=request.form.get("username"), password=pwd_context.encrypt(request.form.get("password")))
        print(result)
        if result != 0:
            # query database for username
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            if len(rows) == 1:
                # remember which user has logged in
                session["user_id"] = rows[0]["id"]
                # redirect user to home page
                return redirect(url_for("index"))
        else:
            return apology("Error happened while registration, please try again")

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
    

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        formSymbol=request.form['symbol']
        formShares=request.form['shares']
        deleted = False
        total = 0
        check = db.execute("SELECT shares FROM shares WHERE userid = :userid AND symbol = :symbol", userid=session["user_id"], symbol=formSymbol)
        if int(check[0]['shares']) - int(formShares) >= 0:
            sharesPrice = lookup(formSymbol).get('price') * float(formShares)
            #True if there are no shares left after sell
            if int(check[0]['shares']) - int(formShares) == 0:
                resultShares = db.execute("DELETE FROM shares WHERE userid = :userid and symbol = :symbol", userid=session["user_id"], symbol=formSymbol)
                deleted = True
            else:    
                resultShares = db.execute("UPDATE shares SET shares=shares-:shares WHERE userid = :userid AND symbol = :symbol", userid=session["user_id"], symbol=formSymbol, shares= formShares)                   
            # update query to change users cash
            resultUpdate = db.execute("UPDATE users SET cash=cash+" + str(sharesPrice) +" WHERE id=" + str(session["user_id"]))
            #Checking if both queries were successful
            if resultShares != 0 and resultUpdate != 0:
                #Insert row to history table
                db.execute("INSERT INTO history (userid,symbol,shares,price,kind,date) VALUES (:userid,:symbol,:shares,:price,:kind,:date)",userid=session["user_id"],symbol=formSymbol,shares=formShares,price=usd(lookup(formSymbol).get('price')),kind="SELL",date=time.strftime('%Y-%m-%d %H:%M:%S'))
                if deleted == False:
                    #Get updated shares and total
                    check = db.execute("SELECT shares FROM shares WHERE userid = :userid AND symbol = :symbol", userid=session["user_id"], symbol=formSymbol)
                    shares = int(check[0]['shares'])
                    total=lookup(formSymbol).get('price') * shares
                #Return json to frontend (will handle it on frontend)
                return jsonify(success=True,total=usd(total))
            else:
                #Return json to frontend (will handle it on frontend)
                return jsonify(success=False,message="Error while making Database changes.")
        else:
            #Return json to frontend (will handle it on frontend)
                return jsonify(success=False,message="You are trying to sell more shares than you have. Maximum number of shares you can sell is " + str(check[0]['shares']))
                 
    else:
        result = db.execute("SELECT symbol FROM shares WHERE userid = :userid", userid=session["user_id"])
        if len(result) != 0:
            return render_template("sell.html",shares=result)
        else:
            return render_template("sell.html")

