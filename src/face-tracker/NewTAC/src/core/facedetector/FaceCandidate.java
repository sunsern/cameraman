package core.facedetector;

public class FaceCandidate {

	public int x, y, size, weight;

	public FaceCandidate(int tx, int ty, int tsize) {
		x = tx;
		y = ty;
		size = tsize;
		weight = 1;
	}

	public boolean isSizeSimilar(Object obj) {

		if (this.getClass() != obj.getClass()) return false;

		FaceCandidate other = (FaceCandidate) obj;

		if (other.size > 3 * this.size / 2 || other.size < 2 * this.size / 3) return false;
		
		return true;

	}

	public boolean isContaining(Object obj) {
		if (this.getClass() != obj.getClass()) return false;

		FaceCandidate other = (FaceCandidate) obj;

		int tx1 = this.x;
		int ty1 = this.y;
		int tx2 = this.x + this.size - 1;
		int ty2 = this.y + this.size - 1;
		
		int ox1 = other.x;
		int oy1 = other.y;
		int ox2 = other.x + other.size - 1;
		int oy2 = other.y + other.size - 1;
		
		if ((ox1 >= tx1 && ox1 <= tx2	&& oy1 >= ty1 && oy1 <= ty2) && 
				(ox2 >= tx1	&& ox2 <= tx2 && oy1 >= ty1 && oy1 <= ty2) && 
				(ox1 >= tx1 && ox1 <= tx2	&& oy2 >= ty1 && oy2 <= ty2) && 
				(ox2 >= tx1	&& ox2 <= tx2	&& oy2 >= ty1 && oy2 <= ty2)) {
			return true;
		}
		
		return false;
	}

	public boolean isOverlap(Object obj) {
		if (this.getClass() != obj.getClass()) return false;

		FaceCandidate other = (FaceCandidate) obj;

		int tx1 = this.x;
		int ty1 = this.y;
		int tx2 = this.x + this.size - 1;
		int ty2 = this.y + this.size - 1;
		
		int ox1 = other.x;
		int oy1 = other.y;
		int ox2 = other.x + other.size - 1;
		int oy2 = other.y + other.size - 1;
		
		if ((ox1 >= tx1 && ox1 <= tx2	&& oy1 >= ty1 && oy1 <= ty2) || 
				(ox2 >= tx1	&& ox2 <= tx2 && oy1 >= ty1 && oy1 <= ty2) || 
				(ox1 >= tx1 && ox1 <= tx2	&& oy2 >= ty1 && oy2 <= ty2) || 
				(ox2 >= tx1	&& ox2 <= tx2	&& oy2 >= ty1 && oy2 <= ty2)) {
			return true;
		}
		
		return false;
	}
	
	
	public boolean isSameDetection(Object obj) {
		
		if (this.getClass() != obj.getClass()) return false;
		
		FaceCandidate other = (FaceCandidate) obj;

		int tx1 = this.x;
		int ty1 = this.y;
		int tx2 = this.x + this.size - 1;
		int ty2 = this.y + this.size - 1;
		
		int ox1 = other.x;
		int oy1 = other.y;
		int ox2 = other.x + other.size - 1;
		int oy2 = other.y + other.size - 1;
		
		int x1 = Math.max(tx1, ox1);
		int y1 = Math.max(ty1, oy1);
		
		int x2 = Math.min(tx2, ox2);
		int y2 = Math.min(ty2, oy2);
		
		if (this.isOverlap(other) || other.isOverlap(this)) {
			int this_area = this.size * this.size;
			int other_area = other.size * other.size;
			int intersect_area = (x2 - x1 + 1) * (y2 - y1 + 1);
			
			if (intersect_area > 2 * Math.min(this_area, other_area) / 3) {
				return true;
			}
		}
		return false;
	}
	
}
