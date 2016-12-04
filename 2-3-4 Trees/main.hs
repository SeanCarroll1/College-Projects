
data Tree t = Empty
                 | Two t (Tree t) (Tree t)
                 | Three (t,t) (Tree t) (Tree t) (Tree t)
                 | Four (t,t,t) (Tree t) (Tree t) (Tree t) (Tree t)
                 deriving (Eq, Ord,Show)

--trees for testing
mytrees = Two 15 (Four (2,4,8) (Two 1 Empty Empty) (Two 3 Empty Empty) (Two 6 Empty Empty) (Two 9 Empty Empty)) (Two 20 Empty (Two 25 Empty Empty))
test=Two 10(Three (4,5)(Two 1 Empty Empty) (Two 3 Empty Empty) (Two 6 Empty Empty)) (Two 20 Empty (Two 25 Empty Empty))
tree1=insert 10 mytrees
tree2=insert 11 tree1
tree3=insert 12 tree2
tree4=insert 14 tree3
tree6=insert 16 tree4
					
-- print out of tree							
display = putStrLn $ printOut tree3
									  
printOut :: Show t => Tree t -> String
printOut x = printOut' x 0 
 where 
    printOut' x lPos  = case x of
      Two num left right -> (replicate (2 * lPos) ' ') ++ show num ++ "\n" ++ (printOut' left (lPos  ))++"  "++ (printOut' right (lPos +1))++"\n"
      Three (num,num2) left mid right ->  (replicate (2 * lPos) ' ') ++show num++ "," ++show num2 ++ "\n" ++ (printOut' left (lPos )) ++ "  " ++ (printOut' mid (lPos+1 ))++ "  " ++ (printOut' right (lPos +1))++"\n"
      Four (num,num2,num3) left lMid rMid right -> (replicate (2 * lPos) ' ') ++show num++","++show num2 ++","++ show num3++ "\n" ++ (printOut' left (lPos)) ++ "  " ++ (printOut' lMid (lPos+1))++ "   " ++ (printOut' rMid (lPos+1))++ "  " ++ (printOut' right (lPos+1))++"\n"
      Empty -> (replicate (2 * lPos) ' ') ++ "Empty" 
	   
 --search method        
check :: (Ord a) =>a ->Tree a ->Bool

check _ Empty = False
 
check x (Two y left right)
          | x == y = True
          | x<y = check x left
		  |x > y = check  x right
		  
check x (Three (y,z) left mid right)
          |x == y ||x == z = True
          |x < y = check x left
		  |x > z = check x right
		  |otherwise = check x mid

check x (Four (y,z,a) left lmid rmid right)
          |x == y ||x == z || x==a = True
          |x < y = check x left
		  |x > a = check x right
		  |x>z =check x rmid
		  |otherwise = check x lmid

--insertion method
insert :: Ord t => t -> Tree t -> Tree t

insert x Empty = Two x Empty Empty

insert x (Two y Empty Empty)
          | x > y = Three (y,x) Empty Empty Empty
          | otherwise = Three (x,y) Empty Empty Empty

insert x (Three (y,z) Empty Empty Empty)
          | x <= y = Four (x,y,z) Empty Empty Empty Empty		  
		  | x > z = Four (y,z,x) Empty Empty Empty Empty
          | otherwise = Four (y,x,z) Empty Empty Empty Empty

insert x (Two y left right)
          | x <= y = case left of (Four (a,b,c) lNode lMidNode rMidNode rNode) -> if x <= b then Three (b,y) (insert x (Two a lNode lMidNode)) (Two c rMidNode rNode) right
                                                                           else Three (b,y) (Two a lNode lMidNode) (insert x (Two c rMidNode rNode)) right
                                  left -> Two y (insert x left) right
          | otherwise = 
              case right of (Four (a,b,c) lNode lMidNode rMidNode rNode) -> if x <= b then Three (y,b) left (insert x (Two a lNode lMidNode)) (Two c rMidNode rNode)
                                                                            else Three (y,b) left (Two a lNode lMidNode) (insert x (Two c rMidNode rNode))
                            right ->  Two y left (insert x right)

insert x (Three (a,b) left middle right)
          | x <= a =  Three (a,b) (insert x left) middle right
          | x > a && x <= b =  Three (a,b) left (insert x middle) right
          | x>b = Three (a,b) left middle (insert x right)
				
insert x (Four (a,b,c) left mleft mright right)
          | x <= b = Two b (insert x (Two a left mleft)) (Two c mright right)
          |x>b = Two b (Two a left mleft) (insert x (Two c mright right))

