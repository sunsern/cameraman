public class Candidate {

	public int x, y, size, weight;

	public Candidate(int tx, int ty, int tsize) {
		x = tx;
		y = ty;
		size = tsize;
		weight = 1;
	}

	public boolean isSizeSimilar(Object obj) {
		
		if (this.getClass() != obj.getClass())
			return false;
		
		Candidate other = (Candidate) obj;

		if (other.size > 3 * this.size / 2 || other.size < 2 * this.size / 3)
			return false;
		else
			return true;
	
	}

	public boolean isContaining(Object obj) {
		if (this.getClass() != obj.getClass())
			return false;

		Candidate other = (Candidate) obj;

		if ((other.x >= this.x && other.x <= this.x + this.size &&
    		 other.y >= this.y && other.y <= this.y + this.size)  &&
			(other.x+other.size >= this.x && other.x+other.size <= this.x + this.size &&
			 other.y >= this.y && other.y <= this.y + this.size)  &&
			(other.x >= this.x && other.x <= this.x + this.size &&
			 other.y+other.size >= this.y && other.y+other.size <= this.y + this.size)  &&
			(other.x+other.size >= this.x && other.x+other.size <= this.x + this.size &&
			other.y+other.size >= this.y && other.y+other.size <= this.y + this.size)) {
			return true;
			
		}
		else 
			return false;
	
	}
	
	public boolean isOverlap(Object obj) {
		if (this.getClass() != obj.getClass())
			return false;

		Candidate other = (Candidate) obj;

		if ((other.x >= this.x && other.x <= this.x + this.size &&
    		 other.y >= this.y && other.y <= this.y + this.size)  ||
			(other.x+other.size >= this.x && other.x+other.size <= this.x + this.size &&
			 other.y >= this.y && other.y <= this.y + this.size)  ||
			(other.x >= this.x && other.x <= this.x + this.size &&
			 other.y+other.size >= this.y && other.y+other.size <= this.y + this.size)  ||
			(other.x+other.size >= this.x && other.x+other.size <= this.x + this.size &&
			other.y+other.size >= this.y && other.y+other.size <= this.y + this.size)) {
			return true;
			
		}
		else 
			return false;
	
	}
}
