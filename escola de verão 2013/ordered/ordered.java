import java.util.Random;
import java.util.Scanner;

public class ordered {
    
    static Random random = new Random();
    
    public static class Treap {
        
        int key;
        int first;
        int last;
        int sz;
        boolean inc;
        boolean dec;
        
        long pr;
        
        Treap left;
        Treap right;
        
        Treap(int value) {
            key = value;
            left = right = null;
            first = last = value;
            inc = dec = true;
            sz = 1;
            pr = random.nextLong();
        }
        
        void fix() {
            sz = 1 + getSize(left) + getSize(right);
            
            boolean linc = true, ldec = true;
            boolean rinc = true, rdec = true;
            int llast = key, rfirst = key;
            
            if (left != null) {
                first = left.first;
                llast = left.last;
                linc = left.inc;
                ldec = left.dec;
            } else {
                first = key;
            } 
            
            if (right != null) {
                last = right.last;
                rfirst = right.first;
                rinc = right.inc;
                rdec = right.dec;
            } else {
                last = key;
            }
            
            inc = linc && (llast <= key) && (key <= rfirst) && rinc;
            dec = ldec && (llast >= key) && (key >= rfirst) && rdec;
        }
    }
    
    static int getSize(Treap root) {
        return (root == null) ? 0 : root.sz;
    }    
    
    static class TreapPair {
        Treap left;
        Treap right;
        
        TreapPair(Treap left, Treap right) {
            this.left = left;
            this.right = right;
        }
    } 
    
    static TreapPair split(Treap root, int pos) {
        if (root == null) {
            return new TreapPair(null, null);
        }

        if (pos <= getSize(root.left)) {
            TreapPair sub = split(root.left, pos);
            root.left = sub.right;
            root.fix();
            sub.right = root;
            return sub;
        } else {
            TreapPair sub = split(root.right, pos - getSize(root.left) - 1);
            root.right = sub.left;
            root.fix();
            sub.left = root;
            return sub;
        }
    }
    
    static Treap merge(Treap left, Treap right) {
        if (left == null)
            return right;
        if (right == null)
            return left;
        if (left.pr > right.pr) {
            left.right = merge(left.right, right);
            left.fix();
            return left;
        } else {
            right.left = merge(left, right.left);
            right.fix();
            return right;
        }
    }
    
    static Treap insert(Treap root, int pos, int x) {
        TreapPair t = split(root, pos);
        return merge(merge(t.left, new Treap(x)), t.right);
    }
    
    static Treap remove(Treap root, int pos) {
        TreapPair t = split(root, pos);
        return merge(t.left, split(t.right, pos + 1 - getSize(t.left)).right);
    }
    
    static Treap modify(Treap root, int a, int newkey) {
      TreapPair t1 = split(root, a + 1);
      TreapPair t2 = split(t1.left, a);
      t2.right.key = newkey;
      t2.right.fix();
      return merge(merge(t2.left, t2.right), t1.right);
    }
    
    static int ith(Treap root, int k) {
        if (k < getSize(root.left))
            return ith(root.left, k);
        else if (k > getSize(root.left))
            return ith(root.right, k - getSize(root.left) - 1);
        return root.key;
    }
    
    static class TreapAndResult {
      Treap treap;
      String value;

      TreapAndResult(Treap t, String value) {
        this.treap = t;
        this.value = value;
      }
    }
    
    static TreapAndResult query(Treap root, int a, int b) {
      
      TreapPair t1 = split(root, b + 1);
      TreapPair t2 = split(t1.left, a);
      boolean inc = t2.right.inc;
      boolean dec = t2.right.dec;
      String res = "NONE";
      
      if (inc && dec) res = "ALL EQUAL";
      else if (inc) res = "NON DECREASING";
      else if (dec) res = "NON INCREASING";

      return new TreapAndResult(merge(merge(t2.left, t2.right), t1.right), res);
    }
    
    static void print(Treap root) {
      if (root == null)
        return;
      print(root.left);
      System.err.print(root.key + " ");
      print(root.right);
    }
    
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int N, Q, ox, oy, cmd, x, y;
        
        while (in.hasNextInt()) {
            N = in.nextInt();
            
            Treap treap = null;
            for (int i = 0; i < N; ++i) {
                x = in.nextInt();
                treap = insert(treap, i, x);
            }

            Q = in.nextInt();
            while (Q-->0) {
                cmd = in.nextInt();
                switch (cmd) {
                    case 0:
                        x = in.nextInt(); y = in.nextInt();
                        ox = ith(treap, x-1);
                        oy = ith(treap, y-1);
                        treap = modify(treap, x-1, oy);
                        treap = modify(treap, y-1, ox);
                    break;
                    case 1:
                        x = in.nextInt(); y = in.nextInt();
                        treap = modify(treap, x-1, y);
                    break;
                    case 2:
                        x = in.nextInt(); y = in.nextInt();
                        treap = insert(treap, x-1, y);
                    break;
                    case 3:
                        x = in.nextInt();
                        treap = remove(treap, x-1);
                    break;
                    case 4:
                        x = in.nextInt(); y = in.nextInt();
                        TreapAndResult tr = query(treap, x-1, y-1);
                        treap = tr.treap;
                        String v = tr.value;
                        System.out.println(v);
                    break;
                }
            }
        }
    }
}