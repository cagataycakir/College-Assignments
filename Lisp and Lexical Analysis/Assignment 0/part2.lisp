(setq ifp (open "boundries.txt" :if-does-not-exist nil))
(setq ofp (open "primedistribution.txt" :direction :output))

(defun get-list-from-file (fp)
	(read-from-string
		(concatenate 'string "(" (read-line fp) ")")
	)
)

(defun is-prime (num)
	"Finds if the given number is a prime"
    (if (> num 1)
        (let ((m 2) (p t))
            (loop while (< m (/ num 2)) do
                (if (=(rem num m) 0) (setq p NIL))
                (incf m)
            )
            p
        )
        NIL
    )
)

;find if a given number is semi prime
;if it is already prime it is not semiprime
;if it's divider and division is both prime our num is semiprime
(defun is-semiprime (num)
	"Finds if the given number is a semiprime"
    (if (is-prime num)
        NIL
        (let ((m 2) (flag NIL) (sp NIL))
            (loop while (and (not flag)(< m num)) do ;get out of loop if divider is found (set flag)
                (if (and (is-prime m) (=(rem num m) 0))
                    (progn
                        (setq flag t)
                        (if (is-prime (/ num m))
                            (setq sp t)
                        )
                    )
                    (incf m)
                )
            )
            sp
        )
    )
)

(defun list-primes-in-scope-to-file (num1 num2 fp)
	(loop for num from num1 to num2 do
		(if (is-prime num)
			(format fp "~D is prime~%" num)
			(if (is-semiprime num)
				(format fp "~D is semiprime~%" num)
			)
		)
	)
)

(if (and ifp ofp)
	(let	( (twonum (get-list-from-file ifp)) )
		(if (and (= (list-length twonum) 2)(> (nth 0 twonum) 1) (> (nth 1 twonum) 1) )
			(list-primes-in-scope-to-file (nth 0 twonum) (nth 1 twonum) ofp)
			(print "Input 2 numbers that are greater than 1.")
		)
		(close ifp)
		(close ofp)
		(exit)
	)
)

(write-line "Invalid file(s).")

(if ifp (close ifp))
(if ofp (close ofp))