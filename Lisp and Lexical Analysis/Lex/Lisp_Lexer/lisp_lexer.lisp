(defconstant lcLet (coerce "abcdefghijklmnopqrstuvwxyz" 'list))
(defconstant ucLet (coerce "ABCDEFGHIJKLMNOPQRSTUVWXYZ" 'list))
(defconstant nums (coerce "0123456789" 'list))
(defconstant allowedChr (coerce "=<+-/*" 'list))

;;TOKEN LIST INIT
(setq tokenBuff "") ;is also used for syntax check
(setq tokenList '())
(setq definedId '());to keep track of defined id strings
(setq commentF nil)
(setq stringF nil)
;;TOKEN LIST INIT

;;TOKEN SYNTAX CHECK
(setq deffunID '())
(setq defvalID '())
(setq defstavalID '())
;;TOKEN SYNTAX CHECK

;;TOKEN LIST INIT FUNCS
(defun add-to-definedId()
	(push tokenBuff definedId)
)

(defun is-var (numstring)
	"check if a string is a valid var"
	(let ((dotflag t))
		(loop for num in (coerce numstring 'list) do
			(if (char= num #\.)
				(if dotflag
					(setq dotflag nil)
					(handle_ERR (concatenate 'string "'" numstring "'" " is not a valid value")))
				(if (not (find num nums))
					(return-from is-var nil))))
		(if (string= numstring ".") (handle_ERR "'.' is not a valid value") t)))

(defun is-valid-id (idstring)
	(if (find (char idstring 0) nums)
		(handle_ERR (concatenate 'string "'" idstring "'" " is not a valid identifier")))
	t
)

(defun handle_ERR (string)
	(format t "SYNTAX_ERROR ~a~%" string)
	(exit)
)

(defun token-recog (input)
	(if (string-equal input "and") (return-from token-recog "KW_AND"))
	(if (string-equal input "or") (return-from token-recog "KW_OR"))
	(if (string-equal input "not") (return-from token-recog "KW_NOT"))
	(if (string-equal input "=") (return-from token-recog "KW_EQUAL"))
	(if (string-equal input "<") (return-from token-recog "KW_LESS"))
	(if (string-equal input "for") (return-from token-recog "KW_FOR"))
	(if (string-equal input "if") (return-from token-recog "KW_IF"))
	(if (string-equal input "list") (return-from token-recog "KW_LIST"))
	(if (string-equal input "append") (return-from token-recog "KW_APPEND"))
	(if (string-equal input "concat") (return-from token-recog "KW_CONCAT"))
	(if (string-equal input "set") (return-from token-recog "KW_SET"))
	(if (string-equal input "deffun") (return-from token-recog "KW_DEFFUN"))
	(if (string-equal input "load") (return-from token-recog "KW_LOAD"))
	(if (string-equal input "disp") (return-from token-recog "KW_DISP"))
	(if (string-equal input "exit") (return-from token-recog "KW_EXIT"))
	(if (string-equal input "nil") (return-from token-recog "KW_NIL"))
	(if (string-equal input "true") (return-from token-recog "KW_TRUE"))
	(if (string-equal input "false") (return-from token-recog "KW_FALSE"))
	(if (string-equal input "while") (return-from token-recog "KW_WHILE"))
	(if (string-equal input "+") (return-from token-recog "OP_PLUS"))
	(if (string-equal input "-") (return-from token-recog "OP_MINUS"))
	(if (string-equal input "/") (return-from token-recog "OP_DIV"))
	(if (string-equal input "*") (return-from token-recog "OP_MULT"))
	(if (string-equal input "**") (return-from token-recog "OP_DBLMULT"))
	(if (string-equal input ",") (return-from token-recog "OP_COMMA"))
	(if (is-valid-id input)
		(progn (add-to-definedId) "IDENTIFIER"))
)

(defun add-token-char-and-buff(mychr)
	(add-token-buff)
	(if (char= mychr #\() (setq tokenList (append tokenList (list "OP_OP"))))
	(if (char= mychr #\)) (setq tokenList (append tokenList (list "OP_CP"))))
)

(defun add-token-buff()
	(if (> (length tokenBuff) 0)
		(progn
			(if (is-var tokenBuff) 
				(setq tokenList (append tokenList (list "VALUE")))
				(setq tokenList (append tokenList (list(token-recog tokenBuff))))
			)
			(setq tokenBuff ""))))

(defun parse-character (strchar)
	(if (string= tokenBuff ";")
		(if (string= strchar ";")
			(setq commentF t)
			(handle_ERR "Unknown token ';'");handle syntax error
		)
	)
	(setq tokenBuff (concatenate 'string tokenBuff (list strchar)))
)

(defun fill-token-list (mychlst)
	(loop for strchar in (coerce mychlst 'list)
		do (progn
			(if commentF
				(progn
					(setq tokenBuff (concatenate 'string tokenBuff "COMMENT"))
					(return-from fill-token-list)
				)
				(if (or (not stringF) (char= strchar #\"))
					(case strchar
						(#\" (if stringF
								(progn
									(setq tokenList (append tokenList (list "OP_CC")))
									(setq stringF nil))
								(progn
									(setq tokenList (append tokenList (list "OP_OC" "STRING")))
									(setq stringF t))))
						(#\space (add-token-buff))
						(#\( (add-token-char-and-buff #\())
						(#\) (add-token-char-and-buff #\)))
						(otherwise (parse-character strchar))
					))
			)))
	(if stringF (handle_ERR "Expected '\"'"))
	(add-token-buff)
	(nreverse definedId))

;;TOKEN LIST INIT FUNCS

;;TOKEN SYNTAX CHECK FUNCS
(defun search-id-fun()
	(let ((itemno 0))
		(loop for item in defunID do
			(progn
				(if (string-equal tokenBuff item)(return-from search-id-fun itemno))
				(incf itemno))))
	nil
)

(defun search-id-val()
	(loop for item in defvalID do
		(if (string-equal (car definedId) item)(progn (pop definedId) (return-from search-id-val t))))
	(loop for item in defstavalID do
		(if (string-equal (car definedId) item)(progn (pop definedId) (return-from search-id-val t))))
	(pop definedId)
	nil)

(defun tok-exit()
	(if (not(string-equal (get-token) "KW_EXIT"))
		(handle_ERR "Wrong syntax for 'exit'"))
	(if (not(string-equal (get-token) "OP_CP"))
		(handle_ERR "Wrong syntax for 'exit'"))
)

(defun tok-deffun()
	(if (not(string-equal (get-token) "IDENTIFIER"))
		(handle_ERR "Wrong syntax for 'deffun'"))
	(push (car definedId) deffunID)
	(pop definedId)
	(if (not(string-equal (get-token) "OP_OP"))
		(handle_ERR "Wrong syntax for 'deffun'"))
	(if (not(string-equal (get-token) "IDENTIFIER"))
		(handle_ERR "Wrong syntax for 'deffun'"))
	(push (car definedId) defvalID)
	(pop definedId)
	(if (not(string-equal (get-token) "OP_CP"))
		(handle_ERR "Wrong syntax for 'deffun'"))
	(forms)
	(pop defvalID)
	(if (not(string-equal (get-token) "OP_CP"))
		(handle_ERR "no OP_CP on 'for'"))
)

(defun tok-set()
	(if (not(string-equal (get-token) "IDENTIFIER"))
		(handle_ERR "Wrong syntax for 'set'"))
	(push (car definedId) defstavalID)
	(pop definedId)
	(get-token)
	(is-val)
	(if (not(string-equal (get-token) "OP_CP"))
		(handle_ERR "Wrong syntax for 'set'"))
)

(defun tok-while()
	(if (not(string-equal (get-token) "OP_OP"))
		(handle_ERR "Wrong syntax for 'while'"))
	(get-token)
	(is-val)
	(if (not(string-equal (get-token) "OP_CP"))
		(handle_ERR "Wrong syntax for 'while'"))
	(forms)
	(if (not(string-equal (get-token) "OP_CP"))
		(handle_ERR "Wrong syntax for 'while'"))
)

(defun tok-for()
	(if (not(string-equal (get-token) "OP_OP"))
		(handle_ERR "Wrong syntax for 'for'"))
	(if (not(string-equal (get-token) "IDENTIFIER"))
		(handle_ERR "Wrong syntax for 'for'"))
	(push (car definedId) defvalID)
	(pop definedId)
	(get-token)
	(is-val)
	(if (not(string-equal (get-token) "OP_CP"))
		(handle_ERR "Wrong syntax for 'for'"))
	(forms)
	(pop defvalID)
	(if (not(string-equal (get-token) "OP_CP"))
		(handle_ERR "no OP_CP on 'for'"))
)

(defun tok-if()
	(get-token)
	(is-val)
	(get-token)
	(is-val)
	(if (string-equal (get-token) "OP_CP")
		return
		(is-val)))

(defun tok-con()
	(if (string-equal (get-token) "OP_CP") return)
	(if (string-equal tokenBuff "OP_OP")
		(if (string-equal (get-token) "KW_LIST") (sAuLSV)
			(handle_ERR "S-Expression is not a list at KW_CONCAT"))
		(handle_ERR "S-Expression is not a list at KW_CONCAT"))
	(loop while (string-equal (get-token) "OP_OP")do
		(if (string-equal (get-token) "KW_LIST")
			(progn (setq tokenBuff "KW_CONCAT") (sAuLSV))
			(handle_ERR "S-Expression is not a list at KW_CONCAT")))
	(if (not(string-equal tokenBuff "OP_CP"))
		(handle_ERR "S-Expression is not a list at KW_CONCAT")))

(defun tok-app()
	(if (string-equal (get-token) "OP_CP") return)
	(if (string-equal tokenBuff "OP_OP")
		(if (string-equal (get-token) "KW_LIST") (sAuLSV)
			(handle_ERR "S-Expression is not a list at KW_APPEND"))
		(handle_ERR "S-Expression is not a list at KW_APPEND")
	)
	(setq tokenBuff "KW_APPEND")
	(sAuLV)
)

(defun is-val()
	(if (string-equal tokenBuff "OP_OP")
		(if (string-equal (get-token) "KW_LIST")
			(handle_ERR (concatenate 'string tokenBuff " is not a value"))
			(func-tok))
		(if (not (or	(string-equal tokenBuff "VALUE")
						(search-id-val)))
			(handle_ERR (concatenate 'string tokenBuff " is not a value")))))

(defun uV()
	(let ((temp tokenBuff))
		(get-token)
		(is-val)
		(loop while (and (not(string-equal (get-token) ""))
						 (not(string-equal tokenBuff "OP_CP")))do
			(is-val)
		)
		(if (not(string-equal tokenBuff "OP_CP"))
			(handle_ERR (concatenate 'string "S-Expression is not closed at " temp)))))

(defun sAuV()
	(let ((temp tokenBuff))
		(loop while (and (not(string-equal (get-token) ""))
						 (not(string-equal tokenBuff "OP_CP")))do
			(is-val)
		)
		(if (not(string-equal tokenBuff "OP_CP"))
			(handle_ERR (concatenate 'string "S-Expression is not closed at " temp)))))

(defun sLSV()
	(get-token)
	(if (string-equal tokenBuff "OP_OP")	(open-par-loop)
	(if (string-equal tokenBuff "STRING")	(string-parse)
	(if (not (string-equal tokenBuff "VALUE")
		(handle_ERR (concatenate 'string "Unexpected token " tokenBuff))))))
	(get-token)
	(if (not(string-equal tokenBuff "OP_CP"))
		(handle_ERR (concatenate 'string "S-Expression is not closed at " tokenBuff)))
)


(defun sAuLSV()
	(let ((temp tokenBuff))
		(loop while (and (not(string-equal (get-token) ""))
						 (not(string-equal tokenBuff "OP_CP"))) do
			(if (not(string-equal tokenBuff "VALUE"))
			(if (string-equal tokenBuff "OP_OP")(open-par-loop)
			(if (string-equal tokenBuff "OP_OC")(string-parse)
			(handle_ERR (concatenate 'string "Unexpected token at " temp))
			))))
		(if (not(string-equal tokenBuff "OP_CP"))
			(handle_ERR (concatenate 'string "S-Expression is not closed at " temp)))))
(defun sAuLV()
	(let ((temp tokenBuff))
		(loop while (and (not(string-equal (get-token) ""))
						 (not(string-equal tokenBuff "OP_CP"))) do
			(if (not(string-equal tokenBuff "VALUE"))
			(if (string-equal tokenBuff "OP_OP")(open-par-loop)
			(handle_ERR (concatenate 'string "Unexpected token at " temp))
			)))
		(if (not(string-equal tokenBuff "OP_CP"))
			(handle_ERR (concatenate 'string "S-Expression is not closed at " temp)))))
			
(defun get-token()
	(setq tokenBuff (car tokenList))
	(setq tokenList (cdr tokenList))
	(print tokenBuff)
	tokenBuff
)

(defun func-tok()
	(if (or (string-equal tokenBuff "KW_AND")
			(string-equal tokenBuff "KW_OR")
			(string-equal tokenBuff "KW_LIST"))		(sAuLSV)
	(if (or (string-equal tokenBuff "KW_EQUAL")
			(string-equal tokenBuff "OP_MINUS")
			(string-equal tokenBuff "OP_DIV"))		(uV)
	(if (or	(string-equal tokenBuff "KW_NOT")
			(string-equal tokenBuff "KW_DISP"))		(sLSV)
	(if (or	(string-equal tokenBuff "OP_PLUS")
			(string-equal tokenBuff "OP_MULT")
			(string-equal tokenBuff "OP_DBLMULT"))	(sAuV)
	(if 	(string-equal tokenBuff "KW_APPEND") 	(tok-app)
	(if 	(string-equal tokenBuff "KW_CONCAT") 	(tok-con)
	(if 	(string-equal tokenBuff "KW_IF")	 	(tok-if)
	(if 	(string-equal tokenBuff "KW_FOR") 		(tok-for)
	(if 	(string-equal tokenBuff "KW_WHILE") 	(tok-while)
	(if 	(string-equal tokenBuff "KW_SET") 		(tok-set)
	(if 	(string-equal tokenBuff "KW_DEFFUN") 	(tok-deffun)
	(if 	(string-equal tokenBuff "KW_EXIT")	 	(tok-exit)
	(if 	(string-equal tokenBuff "IDENTIFIER")	(tok-ident)
			(handle_ERR "Token is not valid")
	)))))))))))))
			
)

(defun open-par-loop()
	(get-token)
	(if (string-equal tokenBuff "OP_CP") (return-from open-par-loop))
	(if (or 
			(string-equal tokenBuff "VALUE")
			(string-equal tokenBuff "KW_NIL")
			(string-equal tokenBuff "KW_TRUE")
			(string-equal tokenBuff "KW_FALSE")
			(string-equal tokenBuff "OP_OC")
			(string-equal tokenBuff "OP_CC")
			(string-equal tokenBuff "OP_OP"))
		(handle_ERR "S-Expressions should start with a keyword or a function"))
	(if (string-equal tokenBuff "IDENTIFIER")
		(if (search-id-fun)
			(func-tok)
			(handle_ERR "this identifier is not a function definition"))
		(func-tok)
	)
)

(defun string-parse()
	(get-token)
	(get-token)
)

(defun forms()
	(if (not tokenList) (return-from forms))
	(get-token)
	(if (string-equal tokenBuff "OP_OP") (open-par-loop)
	(if (string-equal tokenBuff "OP_OC") (string-parse)
	(if (not (or
			(string-equal tokenBuff "VALUE")
			(string-equal tokenBuff "KW_NIL")
			(string-equal tokenBuff "KW_TRUE")
			(string-equal tokenBuff "KW_FALSE")
			(string-equal tokenBuff "COMMENT")))
		(handle_ERR (concatenate 'string "Unexpected token at form: " tokenBuff))))))
;;TOKEN SYNTAX CHECK FUNCS

(fill-token-list (read-line))
(forms)