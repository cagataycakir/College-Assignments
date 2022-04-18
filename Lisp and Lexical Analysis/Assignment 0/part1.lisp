(setq ifp (open "nested_list.txt" :if-does-not-exist nil))
(setq ofp (open "flattened_list.txt" :direction :output))

(defun remove-sublists (l)
	"Merges every sublist item into one monolithic list"
    (if (> (list-length l) 0)
        (if (listp (car l))
            (append (remove-sublists (car l)) (remove-sublists (cdr l)))
            (cons (car l) (remove-sublists (cdr l)))
        )
    )
)


(if (and ifp ofp)
	(progn
		(write
			(remove-sublists
				(read-from-string
					(concatenate 'string "(" (read-line ifp) ")")
				)
			) :stream ofp
		)
		(close ifp)
		(close ofp)
		(exit)
	)
)

(write-line "Invalid file(s).")

(if ifp (close ifp))
(if ofp (close ofp))