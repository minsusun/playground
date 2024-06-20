using Xunit;
using customService;

namespace customService.Tests
{
    public class SomeTest
    {
        [Fact]
        public void ThrowSomething()
        {
            var service = new CustomService();
            Assert.Throws<NotImplementedException>(() => service.Service((10)));
            // Assert.False(result, "how this can be an answer");
        }
    }
}