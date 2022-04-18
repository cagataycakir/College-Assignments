;generate a character frequency list by reading input file character by character
;generate a node structure with the created frequency list:
;1- consider frequencies as node scores that in a list
;2- simply merge two lowest scores into one heap node, their heap note shall be the total sum
;3- relist the heap node as one of the node scores and do step 2 again until only one node left
;4- you should have a tree structure that where 0 goes to a lower frequency character on the node

(setq ifp (open "paragraph.txt" :if-does-not-exist nil))
(setq ofp (open "huffman_codes.txt" :direction :output))

(defun read-file-i (fp)
	"Takes the input file as list of lines."
	(loop for lines = (read-line fp nil)
		while lines
		collect lines
	)
)

(defun read-file (fp)
	"Takes the input file as a monolithic string."
	(let ((ls (read-file-i fp)) (con ""))
		(loop for x in ls do
			(setq con (concatenate 'string con x))
		)
		con
	)
)

(defun make-freq-list (s)
	"Generates an unorganized frequency list (lower and upper cases differ)"
	(let ( (freql '()) (temp nil))
		(dotimes (x (length s))
			(setq temp (assoc (char s x) freql))
			(if temp
				(rplacd temp (+ (cdr temp) 1))      ;it is already on temp list. inc item
				(push (cons (char s x) 1) freql)    ;add item to temp list
			)
		)
		freql
	)
)

(defun find-minimum (list)
	"Returns array of the minimum value on a dotted list"
    (if (and list (> (list-length list) 1))
        (let ( (min 0) (temp (nth 0 list)) )
            (loop for n from 0 to (- (list-length list) 1) do
                (if (< (cdr (nth n list)) (cdr temp))
                    (progn (setq temp (nth n list)) (setq min n))
                )
            )
            min
        )
        0
    )
)

(defun remove-from-list (list item)
	"Returns a list where a selected array has been removed (does not modify)"
    (if (or (not list) (< item 0))
        nil
        (if (= item 0)
            (cdr list)
            (cons (car list) (remove-from-list (cdr list) (- item 1)))
        )
    )
)

(defun make-huffman-tree (flist)
	"Generates huffman tree with a frequency list"
    (loop while (> (list-length flist) 1) do
          (let ((min1 nil) (min2 nil) (temp (find-minimum flist)))
               (setq min1 (nth temp flist))
               (setq flist (remove-from-list flist temp))
               (setq temp (find-minimum flist))
               (setq min2 (nth temp flist))
               (setq flist (remove-from-list flist temp))
               (if (< (cdr min1) (cdr min2))
                   (push (cons (list (car min1)(car min2)) (+ (cdr min1) (cdr min2))) flist)
                   (push (cons (list (car min2)(car min1)) (+ (cdr min1) (cdr min2))) flist)
               )
          )
    )
    flist
)

(defun push-all (num mlist)
    (dolist (n mlist)
		(push num (cdr n))
	)
    mlist
)

(defun tree-to-encrypt-vals (hufft)
	"Generates encryption values with the given huffman tree"
    (if (listp hufft)
        (let ( (left (nth 0 hufft)) (right (nth 1 hufft)))
			(if (listp left)
				(setq left (push-all 0 (tree-to-encrypt-vals left)))
				(setq left (list (list left 0)))
			)
			(if (listp right)
				(setq right (push-all 1 (tree-to-encrypt-vals right)))
				(setq right (list (list right 1)))
			)
			(setq hufft (concatenate 'list left right))
		)
		hufft
	)
)

(defun order-by-length (mlist)
	(let ((newlist '()) (current 2))
		(loop while(< (list-length newlist) (list-length mlist)) do
			(loop for x in mlist do
				(if (= (list-length x) current)
					(push x newlist)
				)
			)
			(incf current)
		)
		(reverse newlist)
	)
)

(defun print-huff-code-to-file (huffar fp)
	(setq huffar (order-by-length huffar))
	(loop for x in huffar do
		(format fp "~A:" (car x))
		(loop for y in (cdr x) do
			(format fp " ~D" y)
		)
		(format fp "~%")
	)
)

(if (and ifp ofp)
	(let ((myhuff (caar (make-huffman-tree (make-freq-list (string-upcase (read-file ifp))))) ))
		(print-huff-code-to-file (tree-to-encrypt-vals myhuff) ofp)
		(close ifp)
		(close ofp)
		(exit)
	)
)

(if ifp (close ifp))
(if ofp (close ofp))