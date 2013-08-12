auto void FUNC(TYPE *b, size_t n, TYPE **tp);

void FUNC(TYPE *b, size_t n, TYPE **tp)
{
  /* tp[0] .. tp[n - 1] is now sorted, copy around entries of
     the original array.  Knuth vol. 3 (2nd ed.) exercise 5.2-10.  */
  TYPE *kp;
  TYPE *ip;
  size_t i;
  for (i = 0, ip = b; i < n; i++, ip += SIZE)
    if ((kp = tp[i]) != ip)
      {
	size_t j = i;
	TYPE *jp = ip;
	TYPE t[SIZE];
	memcpy (t, ip, SIZE * sizeof(TYPE));

	do
	  {
	    size_t k = (kp - b) / SIZE;
	    tp[j] = jp;
	    memcpy (jp, kp, SIZE * sizeof(TYPE));
	    j = k;
	    jp = kp;
	    kp = tp[k];
	  }
	while (kp != ip);

	tp[j] = jp;
	memcpy (jp, t, SIZE * sizeof(TYPE));
      }
}
