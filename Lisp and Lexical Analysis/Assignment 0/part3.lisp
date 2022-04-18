(setq ifp (open "integer_inputs.txt" :if-does-not-exist nil))
(setq ofp (open "collatz_outputs.txt" :direction :output))

(defun get-list-from-file (fp)
	(read-from-string
		(concatenate 'string "(" (read-line fp) ")")
	)
)

(defun collatz-list-from-input (mynum)
	"Generates collatz list of the given number"
	(if (and (> mynum 0) (integerp mynum)) 			;exit with nil if mynum is smaller than 1 or not integer
		(let ( (temp mynum) (outlist '()) )
			(push mynum outlist)
			(loop while (> temp 1) do
				(if (=(rem  temp 2) 0) 
					(setq temp (/ temp 2))			;if even num divide by 2
					(setq temp (+ (* temp 3) 1))	;if odd num multiply by 3 and add 1
				)
				(push temp outlist)					;append temporary number to list
			)
			(reverse outlist)
		)
		nil
	)
)

;(print (collatz-list-from-input 27))

(if (and ifp ofp)
	(let ((testnums (get-list-from-file ifp)))
		(if (listp testnums)
			(let ((limit (list-length testnums)))
				(if (> limit 5) (setq limit 5))
				(loop for x from 0 to (decf limit) do
					(format ofp "~D:" (nth x testnums))
					(loop for y in (collatz-list-from-input (nth x testnums)) do
						(format ofp " ~D" y)
					)
					(format ofp "~%")
				)
			)
		)
		(close ifp)
		(close ofp)
		(exit)
	)
)

(if ifp (close ifp))
(if ofp (close ofp))