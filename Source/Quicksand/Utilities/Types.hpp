#ifndef QSE_TYPES_HPP
#define QSE_TYPES_HPP

namespace Quicksand
{

	class CPoint
	{
	public:
		long x, y;

		// construction
		CPoint( void ) { x = y = 0; }
		CPoint( const long newX, const long newY ) { x = newX; y = newY; }
		CPoint( const CPoint& newCPoint ) { x = newCPoint.x; y = newCPoint.y; }
		CPoint( const CPoint* pNewCPoint ) { x = pNewCPoint->x; y = pNewCPoint->y; }
		CPoint( const POINT& newCPoint ) { x = newCPoint.x; y = newCPoint.y; }

		// assignment
		CPoint& operator=(const CPoint& newCPoint) { x = newCPoint.x; y = newCPoint.y; return (*this); }
		CPoint& operator=(const CPoint* pNewCPoint) { x = pNewCPoint->x; y = pNewCPoint->y; return (*this); }

		// addition/subtraction
		CPoint& operator+=(const CPoint& newCPoint) { x += newCPoint.x; y += newCPoint.y; return (*this); }
		CPoint& operator-=(const CPoint& newCPoint) { x -= newCPoint.x; y -= newCPoint.y; return (*this); }
		CPoint& operator+=(const CPoint* pNewCPoint) { x += pNewCPoint->x; y += pNewCPoint->y; return (*this); }
		CPoint& operator-=(const CPoint* pNewCPoint) { x -= pNewCPoint->x; y -= pNewCPoint->y; return (*this); }
		CPoint operator+(const CPoint& other) { CPoint temp( this ); temp += other; return temp; }
		CPoint operator-(const CPoint& other) { CPoint temp( this ); temp -= other; return temp; }

		// comparison
		bool operator==(const CPoint& other) const { return ((x == other.x) && (y == other.y)); }
		bool operator!=(const CPoint& other) const { return (!((x == other.x) && (y == other.y))); }

		// accessors (needed for Lua)
		long GetX() const { return x; }
		long GetY() const { return y; }
		void SetX( const long newX ) { x = newX; }
		void SetY( const long newY ) { y = newY; }
		void Set( const long newX, const long newY ) { x = newX; y = newY; }

		// somewhat hacky vector emulation (maybe I should just write my own vector class)
		float Length() const { return sqrt( (float)(x*x + y*y) ); }
	};

	inline CPoint operator-(const CPoint& left, const CPoint& right) { CPoint temp( left ); temp -= right; return temp; }


	//---------------------------------------------------------------------------------------------------------------------
	// This class Crepresents a rectangle
	//---------------------------------------------------------------------------------------------------------------------
	class CRect
	{
	public:
		long top, left, right, bottom;

		enum RectCorner { INVALID_CORNER = 0, TOPLEFT_CORNER, TOPRIGHT_CORNER, BOTTOMLEFT_CORNER, BOTTOMRIGHT_CORNER };

		//construction
		CRect( void ) { left = top = right = bottom = 0; }
		CRect( long newLeft, long newTop, long newRight, long newBottom ) { Set( newLeft, newTop, newRight, newBottom ); }
		CRect( const CRect& newRect ) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; }
		CRect( CRect* pNewRect ) { left = pNewRect->left; top = pNewRect->top; right = pNewRect->right; bottom = pNewRect->bottom; }
		CRect( const CPoint& topLeft, const CPoint& bottomRight ) { top = topLeft.y; left = topLeft.x; right = bottomRight.x; bottom = bottomRight.y; }
		CRect( const RECT& newRect ) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; }

		//assignment
		CRect& operator=(const CRect& newRect) { left = newRect.left; top = newRect.top; right = newRect.right; bottom = newRect.bottom; return (*this); }
		CRect& operator=(const CRect* pNewRect) { left = pNewRect->left; top = pNewRect->top; right = pNewRect->right; bottom = pNewRect->bottom; return (*this); }

		// addition/subtraction
		CRect& operator+=(const CRect& newRect) { left += newRect.left; top += newRect.top; right += newRect.right; bottom += newRect.bottom; return (*this); }
		CRect& operator-=(const CRect& newRect) { left -= newRect.left; top -= newRect.top; right -= newRect.right; bottom -= newRect.bottom; return (*this); }
		CRect& operator+=(const CRect* pNewRect) { left += pNewRect->left; top += pNewRect->top; right += pNewRect->right; bottom += pNewRect->bottom; return (*this); }
		CRect& operator-=(const CRect* pNewRect) { left -= pNewRect->left; top -= pNewRect->top; right -= pNewRect->right; bottom -= pNewRect->bottom; return (*this); }
		CRect operator+(CRect& other) { CRect temp( this ); temp += other; return temp; }
		CRect operator-(CRect& other) { CRect temp( this ); temp -= other; return temp; }

		// move a rectangle by an amount defined by a CPoint, keeping the rectanle's size constant
		CRect& operator+=(const CPoint& delta) { left += delta.x; top += delta.y; right += delta.x; bottom += delta.y; return (*this); }
		CRect& operator-=(const CPoint& delta) { left -= delta.x; top -= delta.y; right -= delta.x; bottom -= delta.y; return (*this); }
		CRect& operator+=(const CPoint* pDelta) { left += pDelta->x; top += pDelta->y; right += pDelta->x; bottom += pDelta->y; return (*this); }
		CRect& operator-=(const CPoint* pDelta) { left -= pDelta->x; top -= pDelta->y; right -= pDelta->x; bottom -= pDelta->y; return (*this); }
		CRect operator+(CPoint& delta) { CRect temp( this ); temp += delta; return temp; }
		CRect operator-(CPoint& delta) { CRect temp( this ); temp -= delta; return temp; }

		//comparison
		bool operator==(const CRect& other) const { return ((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom)); }
		bool operator!=(const CRect& other) const { return (!((left == other.left) && (top == other.top) && (right == other.right) && (bottom == other.bottom))); }

		// collision
		bool Collide( const CRect& other ) const { if (other.bottom < top || other.right < left || other.left > right || other.top > bottom) { return false; } else { return true; } }
		bool Collide( const CPoint& p ) const { if (p.x > left && p.x < right && p.y < bottom && p.y > top) { return true; } else { return false; } }
		bool IsWithin( const CRect& other ) const { return ((left >= other.left && top >= other.top && right <= other.right && bottom <= other.bottom) || (other.left >= left && other.top >= top && other.right <= right && other.bottom <= bottom)); }
		bool IsWithin( const CPoint& other ) const { return (other.x >= left && other.x <= right && other.y >= top && other.y <= bottom); }
		bool IsValid( void ) const { return (left <= right && top <= bottom); }
		bool IsNull( void ) const { return (left == 0 && right == 0 && top == 0 && bottom == 0); }

		// convenience functions
		void ShiftX( int dx ) { left += dx; right += dx; }
		void ShiftY( int dy ) { top += dy; bottom += dy; }
		void SetX( int x ) { int dx = x - left; ShiftX( dx ); }
		void SetY( int y ) { int dy = y - top; ShiftY( dy ); }

		// accessors for Lua
		long GetTop( void ) const { return top; }
		long GetLeft( void ) const { return left; }
		long GetRight( void ) const { return right; }
		long GetBottom( void ) const { return bottom; }
		CPoint GetCenter( void ) const
		{
			if (IsValid())
				return (CPoint( left + ((right - left) / 2), top + ((bottom - top) / 2) ));
			QSE_ERROR( "Attempting to get the center of an invalid CRect" );
			return CPoint();
		}
		CPoint TopLeft( void ) const { return CPoint( left, top ); }
		CPoint TopRight( void ) const { return CPoint( right, top ); }
		CPoint BottomLeft( void ) const { return CPoint( left, bottom ); }
		CPoint BottomRight( void ) const { return CPoint( right, bottom ); }
		long GetWidth( void ) const { return right - left; }
		long GetHeight( void ) const { return bottom - top; }
		void Set( long newLeft, long newTop, long newRight, long newBottom ) { left = newLeft; top = newTop; right = newRight; bottom = newBottom; }
		void MoveDelta( long dx, long dy ) { left += dx; top += dy; right += dx; bottom += dy; }
		void MoveDelta( const CPoint deltaCPoint ) { MoveDelta( deltaCPoint.x, deltaCPoint.y ); }
		void MoveTo( long x, long y )
		{
			long width = right - left;
			long height = bottom - top;
			left = x;
			right = left + width;
			top = y;
			bottom = top + height;
		}
		void MoveTo( const CPoint& point ) { MoveTo( point.x, point.y ); }
	};

}
#endif